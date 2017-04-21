package com.yjtc.yjy.mark.unite.utils.recognize.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.yjtc.yjy.mark.unite.utils.recognize.interfaces.ShowResultListener;
import com.yjtc.yjy.mark.unite.utils.recognize.model.Constant;
import com.yjtc.yjy.mark.unite.utils.recognize.recognizer;

import java.util.Timer;
import java.util.TimerTask;

import static android.view.MotionEvent.ACTION_DOWN;
import static android.view.MotionEvent.ACTION_MOVE;
import static android.view.MotionEvent.ACTION_POINTER_DOWN;
import static android.view.MotionEvent.ACTION_UP;

/**
 * Created by marstoken on 16/5/18 0020.
 */
public class GestureSurfaceView extends SurfaceView implements SurfaceHolder.Callback, Runnable {
    /**
     * 容器
     */
    private SurfaceHolder mHolder;
    /**
     * 画布对象
     */
    private Canvas mCanvas;
    /**
     * 画笔
     */
    private Paint mPaint;
    /**
     * 路径轨迹
     */
    public Path mPath;

    //触发定时识别任务
    private Timer tExit;
    private TimerTask task;
    /**
     * UI线程显示结果监听
     */
    private ShowResultListener mListener;
    /**
     * 子线程开启标志
     */
    private boolean mIsDraw;
    /**
     * 是否辨认
     */
    private boolean isRecognizer;
    /**
     * 是否显示结果
     */
    private boolean resultDisplay;
    boolean isTwoPoint;
    /**
     * 动态的轨迹坐标,即控制点坐标
     */
    public float startX, startY, posX, posY;
    /**
     * 笔画数
     */
    public int handWriteCount;
    /**
     * 触屏模式
     */
    private int mode = Constant.MARK_SCORE;
    /**
     * 异步识别线程
     */
    private Thread mThread;
    /**
     * 拼接结果
     */
    private StringBuffer result = new StringBuffer();

    public GestureSurfaceView(Context context) {
        this(context, null, 0);
    }

    public GestureSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public GestureSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {//注意重写onTouch无效
        switch (mode) {
            case Constant.DRAW_MODE:
                return onTouchDrawMode(event);
            // TODO: 2017/4/20 0020
            case Constant.MARK_SCORE:
                return onTouchMarkMode(event);
            default:
                break;
        }
        return super.onTouchEvent(event);
    }

    private boolean onTouchDrawMode(MotionEvent event) {

        return false;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mIsDraw = true;
        mThread = new Thread(this);
        mThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        mIsDraw = false;
        mThread = null;
    }

    @Override
    public void run() {
        while (mIsDraw) {
            //更新前的时间
            long startTime = System.currentTimeMillis();
            synchronized (mHolder) {
                if (null != mCanvas) {
                    mCanvas = mHolder.lockCanvas();//获取当前Canvas对象
                    draw();
                    //mHolder.unlockCanvasAndPost(mCanvas);
                }
            }
            long endTime = System.currentTimeMillis();
            long diffTime = endTime - startTime;
            while (diffTime <= Constant.FRAME) {
                diffTime = (int) (System.currentTimeMillis() - startTime);
                Thread.yield();
            }
        }
    }

    private void draw() {
        try {
            if (mCanvas != null) {
                mCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
                mCanvas.drawPath(mPath, mPaint);//画出轨迹
            }
            if (isRecognizer) {
                recognize();
                /**清除工作*/
                recognizer.recognizer_clear();
                isRecognizer = false;
            }
            if (resultDisplay) {
                show();
                clear();
                resultDisplay = false;
                handWriteCount = 0;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (null != mCanvas) {
                //确保每次都提交成功
                mHolder.unlockCanvasAndPost(mCanvas);
            }
        }
    }

    private void clear() {
        clearResult();
        if (tExit != null) {
            tExit.cancel();
            tExit = null;
            task.cancel();
            task = null;
        }
    }

    private void init(Context context) {
        mHolder = getHolder();
        mHolder.addCallback(this);
        setFocusable(true);//---拥有焦点
        setFocusableInTouchMode(true);//触摸时拥有焦点

        mCanvas = new Canvas();
        mPaint = new Paint();
        mPaint.setColor(Color.RED);
        mPaint.setAntiAlias(true);//抗锯齿
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeCap(Paint.Cap.ROUND);//设置画笔为圆滑
        mPaint.setStrokeWidth(20);
        mPath = new Path();

        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        //识别库初始化
        recognizer.recognizer_init();
        if (context instanceof ShowResultListener) {
            mListener = (ShowResultListener) context;
        }
    }

    private boolean onTouchMarkMode(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();
        recognizer.recognizer_add((int) x, (int) y);
        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case ACTION_DOWN:
                startX = x;
                startY = y;
                resultDisplay = false;
                isRecognizer = false;
                if (tExit != null) {
                    tExit.cancel();
                    tExit = null;
                    task.cancel();
                    task = null;
                }
                mPath.moveTo(x, y);
                break;
            case ACTION_MOVE:
                if (getMinDistance(startX, startY, x, y) > Constant.MINVALUE) {
                    if (!isTwoPoint) {
                        mPath.quadTo(posX, posY, x, y);
                    }
                }
                break;
            case ACTION_POINTER_DOWN:
                isTwoPoint = true;
                break;
            case ACTION_UP:
                if (getMinDistance(startX, startY, x, y) > Constant.MINVALUE && !isTwoPoint) {
                    handWriteCount++;
                    isRecognizer = true;
                    tExit = new Timer();
                    task = new TimerTask() {
                        @Override
                        public void run() {
                            resultDisplay = true;
                            mPath.reset();
                        }
                    };
                    tExit.schedule(task, 500);
                } else {
                    mPath.reset();
                    recognizer.recognizer_clear();
                    clearResult();
                }
                isTwoPoint = false;
                break;
        }
        posX = x;
        posY = y;
        return true;
    }

    public void show() {
         /*打印输出结果,此逻辑根据实际情况自己控制，此处专门对手势识别库的容错处理，/也属于半对*/
        if (!TextUtils.isEmpty(result.toString())) {
            String str_score = result.toString().trim();
            if (str_score.startsWith(".")) {
                //点开头不显示
            } else {
                float score_float = Constant.FALL_SCORE;
                if (str_score.equals("/") || str_score.equals("vx") || str_score.equals
                        ("xv")) {
                    //半对
                    mListener.showResult(score_float / 2 + "");

                } else if (str_score.equals("v")) {
                    //对
                    mListener.showResult(score_float + "");
                } else if (str_score.equals("x") || str_score.equals("xx") || str_score
                        .equals("/x") || str_score
                        .equals("1x")) {
                    //错
                    mListener.showResult(0 + "");
                } else if (!str_score.contains("x") && !str_score.contains("/") &&
                        !str_score.contains("v")) {
                    mListener.showResult(result.toString());
                }
            }
        }
    }

    public void recognize() {
        int index = recognizer.recognizer_classify();
        if (handWriteCount == 1) {
            switch (index) {
                case 0:
                    result.append(0);
                    break;
                case 1:
                    result.append(1);
                    break;
                case 2:
                    result.append(2);
                    break;
                case 3:
                    result.append(3);
                    break;
                case 4:
                    result.append(4);
                    break;
                case 5:
                    result.append(5);
                    handWriteCount--;
                    break;
                case 6:
                    result.append(6);
                    break;
                case 7:
                    result.append(7);
                    break;
                case 8:
                    result.append(8);
                    break;
                case 9:
                    result.append(9);
                    break;
                case 10:
                    result.append("v");
                    break;
                case 11:
                    result.append("x");
                    break;
                case 12:
                    result.append("/");
                    break;
                case 13:
                    if (recognizer.recognizer_getResultCnt() >= 2) {
                        result.append(".");
                    }
                    break;
                case 14:
                    result.append("4");
                    handWriteCount--;
                    break;
                case 15:
                    result.append("x");
                    break;
                case 16:
                default:
                    result.append("");
                    break;
            }
            handWriteCount--;
        }
    }

    public void clearResult() {
        if (result.toString().length() > 0) {
            result.delete(0, result.length());
        }
    }

    private float getMinDistance(float x1, float y1, float x2, float y2) {
        return (float) Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
}

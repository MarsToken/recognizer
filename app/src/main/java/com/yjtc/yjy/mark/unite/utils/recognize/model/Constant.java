package com.yjtc.yjy.mark.unite.utils.recognize.model;

/**
 * Created by marstoken on 2016/5/18 0020.
 */
public class Constant {
    /**
     * 涂鸦绘制模式,辞工更难暂未实现
     */
    public static final int DRAW_MODE = 1;
    /**
     * 打分模式
     */
    public static final int MARK_SCORE = 2;
    /**
     * 画布更新频率，此值太大会造成涂鸦闪烁问题
     */
    public static final int FRAME = 5;
    public static final String T = "result==>";
    /**
     * 注意：此demo仅对对，半对有效
     */
    public static float FALL_SCORE = 100;
    /**
     * 手触摸屏幕move时是别的最小距离
     */
    public static int MINVALUE = 5;
}

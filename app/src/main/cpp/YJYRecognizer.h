#ifndef _R_H_
#define _R_H_

#ifdef __cplusplus
extern "C" {
#endif
    
    //识别 返回定义
#define  CHARACTER_ERROR    -1
    
#define  CHARACTER_ZERO     0
#define  CHARACTER_ONE      1
#define  CHARACTER_TWO      2
#define  CHARACTER_THREE    3
#define  CHARACTER_FOUR_1   4
#define  CHARACTER_FIVE     5
#define  CHARACTER_SIX      6
#define  CHARACTER_SEVEN    7
#define  CHARACTER_EIGHT    8
#define  CHARACTER_NINE     9
#define  CHARACTER_RIGHT    10
#define  CHARACTER_WRONG_1  11
#define  CHARACTER_HALF     12
#define  CHARACTER_POINT    13
    
#define  CHARACTER_FOUR_2   14
#define  CHARACTER_WRONG_2  15
#define  CHARACTER_COUNT    16
    
    typedef struct Dot{
        int x;
        int y;
    }Dot, *LpDot;
    
    typedef struct FeatureNode {
        int index;
        float value;
    }FeatureNode, *LpFeatureNode;
    
    typedef struct Node {
        float x;
        float y;
        int iTimeNum;
    }Node, *LpNode;
    
    typedef struct FeatureNodePair {
        Node first;
        Node last;
    }FeatureNodePair, *LpFeatureNodePair;
    
    typedef struct Resultnote {
        float x;
        float y;
        int iTimenum;
        int iAnalyseTimenum;
    }Resultnote, *LpResultnote;
    
    typedef struct Result
    {
        int iTimeNum;
        int iStorkeNum;
        float x;
        float y;
    }Result, *LpResult;
    
    //初始化 与 逆初始化
    bool recognizer_init();
    bool recognizer_deInit();
    
    //采集轨迹清理
    bool recognizer_clear();
    //采集轨迹点
    bool recognizer_add(int x, int y);
    //识别返回
    int recognizer_classify();
    
    //测试使用
    //获取最终识别方向串
    char* recognizer_getDirectionStr();
    //卓著点个数及x y
    int recognizer_getResultCnt();
    int recognizer_getx(int i);
    int recognizer_gety(int i);
    
#ifdef __cplusplus
}
#endif

#endif

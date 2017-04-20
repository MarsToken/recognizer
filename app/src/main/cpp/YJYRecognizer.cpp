
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "YJYRecognizer.h"
//对比库
#define CHARACTER_MAC_LEN 30
/* ———————————————————  1↖️  2↙️  3↘️  4↗️  —————————————————— */
char zero[][CHARACTER_MAC_LEN] = {"02341", "023412", "012341", "02412", "03214",
    "043214", "0241", "0123412", "03412", "0341",
    "0242", "02342", "034123", "0143214", "023414",
    "0234123", "0342", "042", "0312", "01432", "014321"};
char one[][CHARACTER_MAC_LEN] = {"02", "03", "032", "023", "034"};
char two[][CHARACTER_MAC_LEN] = {"04324", "04323", "0323", "0324", "0424",
    "0423", "043243", "03243", "032143", "032134",
    "0321434", "032434", "0432434", "04321434", "04234",
    "0434"};
char three[][CHARACTER_MAC_LEN] = {"03232", "043232", "0432321", "032321", "04232",
    "0432432", "032132", "042321", "03242", "0321321",
    "032432", "042432", "0432132", "043242", "04242",
    "04342"};
char four_1[][CHARACTER_MAC_LEN] = {"023412", "03412", "02412", "0234123", "034123"};
char four_2[][CHARACTER_MAC_LEN] = {"023", "024", "0234", "034"};
char five[][CHARACTER_MAC_LEN] = {"0232", "032", "02432", "02321", "03432",
    "0321", "0231", "0242", "024321", "0342",
    "043432"};
char six[][CHARACTER_MAC_LEN] = {"02341", "023412", "0241", "012341", "0123412",
    "0231", "02412", "01241", "02342", "023423",
    "0242", "03412", "0234", "02312"};
char seven[][CHARACTER_MAC_LEN] = {"032", "042", "0432", "043"};
char eight[][CHARACTER_MAC_LEN] = {"012324", "0123214", "013214", "023214", "02324",
    "012314", "0123241", "013241", "0132141", "01232141",
    "023241", "01324", "032341", "043241", "0432341",
    "042341", "04323414", "032414", "0432414", "0323414",
    "04241", "042324", "0412324", "0413214", "04123214",
    "043234", "03234", "03214", "02141", "0341",
    "0214", "0124", "03241", "03231", "03231",
    "04231", "043231", "0423414", "0143241", "02314",
    "0232142", "02321412", "042412", "0423214"};
char nine[][CHARACTER_MAC_LEN] = {"012342", "02342", "041232", "0412342", "012343",
    "0123423", "02423", "01342", "01242", "041242",
    "01232", "04232", "023423" ,"0242", "04132",
    "04242", "04123423", "042342", "042424", "0123424",
    "0412323", "0412", "034232", "02343", "04123424",
    "0123434", "0232", "0423"};
char right[][CHARACTER_MAC_LEN] = {"034"};
char half[][CHARACTER_MAC_LEN] = {"04"};
char point[][CHARACTER_MAC_LEN] = {"03", "0"};
char wrong_1[][CHARACTER_MAC_LEN] = {"02143", "0213", "0243", "0342"};
char wrong_2[][CHARACTER_MAC_LEN] = {"03", "02"};

#define  ZERO_LIB_LEN 21
#define  ONE_LIB_LEN 5
#define  TWO_LIB_LEN 16
#define  THREE_LIB_LEN 16
#define  FOUR_1_LIB_LEN 5
#define  FOUR_2_LIB_LEN 4
#define  FIVE_LIB_LEN 11
#define  SIX_LIB_LEN 14
#define  SEVEN_LIB_LEN 4
#define  EIGHT_LIB_LEN 44
#define  NINE_LIB_LEN 28
#define  RIGHT_LIB_LEN 1
#define  HALF_LIB_LEN 1
#define  POINT_LIB_LEN 2
#define  WRONG_1_LIB_LEN 0
#define  WRONG_2_LIB_LEN 2

//内部使用函数声明
void feature_read();

float minimum_distance(const Node *first, const Node *last, Node **best);

void addFeature(int index, float value);

void getVertex(Node *first, Node *last, int id);

void makeBasicFeature(int offset, const Node *first, const Node *last);

float distance(const Node *n1, const Node *n2);

float distance2(const Node *n1);

void makeVertexFeature(int sid);

void AddNotePare(int sid);

void featurenodepair_push_back(FeatureNodePair fp);

void makeMoveFeature(int sid, const Node *first, const Node *last);

void feature_sort();

void classify();

int recognizeResult();

//常量定义
#define CHARACTER_WIDTH  320
#define CHARACTER_HEIGHT 460

#define MAX_CHARACTER_SIZE 50
#define ORI_DOT_MAX_CNT  1000
#define FEATURE_NODE_MAX_CNT  1000
#define FEATURE_NODE_PAIR_MAX_CNT 1000
#define RESULT_MAX_CNT 50
#define PT_SIZE 100
#define PI 3.14159265358979323846
#define MAX_DIRECTIONSTR_CNT 500

#define ORI_DOT_NEW oridots = (LpDot)malloc(ORI_DOT_MAX_CNT*sizeof(Dot));
#define ORI_DOT_RESET memset(oridots, 0, ORI_DOT_MAX_CNT*sizeof(Dot));\
oridotsIndex = 0;

#define FEATURE_NODE_NEW featurenodes = (LpFeatureNode)malloc(FEATURE_NODE_MAX_CNT*sizeof(FeatureNode));
#define FEATURE_NODE_RESET memset(featurenodes, 0, FEATURE_NODE_MAX_CNT*sizeof(FeatureNode));\
featurenodesIndex = 0;

#define FEATURE_NODE_PAIR_NEW featurenodepairs = (LpFeatureNodePair)malloc(FEATURE_NODE_PAIR_MAX_CNT*sizeof(FeatureNodePair));
#define FEATURE_NODE_PAIR_RESET  memset(featurenodepairs, 0, FEATURE_NODE_PAIR_MAX_CNT*sizeof(FeatureNodePair)); \
featurenodepairsIndex = 0;

#define RESULT_NEW results = (LpResult)malloc(RESULT_MAX_CNT*sizeof(Result));
#define RESULT_RESET  memset(results, 0, RESULT_MAX_CNT*sizeof(Result)); \
resultsIndex = 0;

//变量定义

LpDot oridots;
int oridotsIndex;

LpFeatureNode featurenodes;
int featurenodesIndex;

LpFeatureNodePair featurenodepairs;
int featurenodepairsIndex;

Resultnote resultnote[PT_SIZE];

LpResult results;
int resultsIndex;

//Dot oridots[ORI_DOT_MAX_CNT];
//FeatureNode featurenodes[FEATURE_NODE_MAX_CNT];
//FeatureNodePair featurenodepairs[FEATURE_NODE_PAIR_MAX_CNT];
//Result results[RESULT_MAX_CNT];
//int oridotsIndex;
//int featurenodesIndex;
//int featurenodepairsIndex;
//int resultsIndex;

char directionStr[MAX_DIRECTIONSTR_CNT] = {0};
int directionStrIndex = 0;

//*************************  External interface ***********************
bool recognizer_init() {
    ORI_DOT_NEW
    ORI_DOT_RESET
    
    FEATURE_NODE_NEW
    FEATURE_NODE_RESET
    
    FEATURE_NODE_PAIR_NEW
    FEATURE_NODE_PAIR_RESET
    
    RESULT_NEW
    RESULT_RESET
    
    return true;
}

bool recognizer_clear() {
    ORI_DOT_RESET
    FEATURE_NODE_RESET
    FEATURE_NODE_PAIR_RESET
    RESULT_RESET
    
    oridotsIndex = 0;
    featurenodesIndex = 0;
    featurenodepairsIndex = 0;
    resultsIndex = 0;
    return true;
}

bool recognizer_add(int x, int y) {
    if(oridotsIndex > ORI_DOT_MAX_CNT -1)
        return false;
    (*(oridots + oridotsIndex)).x = x;
    (*(oridots + oridotsIndex)).y = y;
    oridotsIndex++;
    return true;
}

int recognizer_classify() {
    if (oridotsIndex <= 2) {
        resultsIndex = 0;
        return CHARACTER_POINT;
    }
    feature_read();
    classify();
    return recognizeResult();
}

int recognizer_getx(int i) {
    if (i < 0 || i > resultsIndex - 1)
        return -1;
    
    return (*(results + i)).x;
}

int recognizer_gety(int i) {
    if (i < 0 || i > resultsIndex - 1)
        return -1;
    
    return (*(results + i)).y;
}

bool recognizer_deInit() {
    if (!oridots) {
        delete oridots;
        oridots = 0;
    }
    if (!featurenodes) {
        delete featurenodes;
        featurenodes = 0;
    }
    if (!featurenodepairs) {
        delete featurenodepairs;
        featurenodepairs = 0;
    }
    if (!results) {
        delete results;
        results = 0;
    }
    
    return true;
}

int recognizer_getResultCnt() {
    return resultsIndex;
}

char *recognizer_getDirectionStr() {
    return directionStr;
}

//*************************  Inside interface ***********************
void featurenode_push_back(FeatureNode f) {
    if(featurenodesIndex > FEATURE_NODE_MAX_CNT -1)
        return;
    *(featurenodes + featurenodesIndex++) = f;
}

void featurenodepair_push_back(FeatureNodePair fp) {
    if(featurenodepairsIndex > FEATURE_NODE_PAIR_MAX_CNT -1)
        return;
    *(featurenodepairs + featurenodepairsIndex++) = fp;
}

void result_push_back(Result r) {
    if(resultsIndex > RESULT_MAX_CNT-1)
        return;
    *(results + resultsIndex++) = r;
}

void feature_read() {
    FEATURE_NODE_RESET
    FEATURE_NODE_PAIR_RESET
    const Node *prev = 0;
    
    // bias term
    {
        FeatureNode f;
        f.index = 0;
        f.value = 1.0;
        featurenode_push_back(f);
    }
    
    LpNode nodes = (LpNode) malloc(oridotsIndex * sizeof(Node));
    memset(nodes, 0, oridotsIndex * sizeof(Node));
    
    int inodeNum = 0;
    for (int i = 0; i < oridotsIndex; i++) {
        (*(nodes + i)).x = (*(oridots + i)).x;
        (*(nodes + i)).y = (*(oridots + i)).y;
        (*(nodes + i)).iTimeNum = 10000 + inodeNum++;
    }
    
    Node *first = nodes;
    Node *last = nodes + oridotsIndex - 1;
    
    getVertex(first, last, 0);
    makeVertexFeature(0);
    
    //AddNotePare(0);
    
    if (prev) {
        makeMoveFeature(0, prev, first);
    }
    prev = last;
    
    
    addFeature(2000000, oridotsIndex);
    addFeature(2000000 + oridotsIndex, 10);
    
    feature_sort();
    
    {
        FeatureNode f;
        f.index = -1;
        f.value = 0.0;
        featurenode_push_back(f);
    }
    
    if (nodes) {
        delete nodes;
        nodes = 0;
    }
}

void getVertex(Node *first, Node *last, int id) {
    
    //补丁 解决6s崩溃问题
    if(id >= oridotsIndex)
        return;
    
    (*(featurenodepairs + id)).first = *first;
    (*(featurenodepairs + id)).last = *last;
    
    if (featurenodepairsIndex <= id)
        featurenodepairsIndex = id + 1;
    
    Node *best = 0;
    const float dist = minimum_distance(first, last, &best);
    
    static const float error = 0.001;
    if (dist > error) {
        getVertex(first, best, id * 2 + 1);
        getVertex(best, last, id * 2 + 2);
    }
}

float minimum_distance(const Node *first, const Node *last, Node **best) {
    if (first == last) return 0.0;
    
    const float a = 1.0 * last->x / 320 - 1.0 * first->x / 320;
    const float b = 1.0 * last->y / 460 - 1.0 * first->y / 460;
    const float c =
    1.0 * last->y / 460 * 1.0 * first->x / 320 - 1.0 * last->x / 320 * 1.0 * first->y / 460;
    
    float max = -1.0;
    for (const Node *n = first; n != last; ++n) {
        const float dist = fabs((a * 1.0 * n->y / 460) - (b * 1.0 * n->x / 320) + c);
        if (dist > max) {
            max = dist;
            *best = const_cast<Node *>(n);
        }
    }
    return max * max / (a * a + b * b);
}

void addFeature(int index, float value) {
    FeatureNode f;
    f.index = index;
    f.value = value;
    featurenode_push_back(f);
}

void makeBasicFeature(int offset,
                      const Node *first,
                      const Node *last) {
    // distance
    addFeature(offset + 1, 10 * distance(first, last));
    
    // degree
    addFeature(offset + 2, atan2(last->y - first->y, last->x - first->x));
    
    // absolute position
    addFeature(offset + 3, 10 * (first->x - 0.5));
    addFeature(offset + 4, 10 * (first->y - 0.5));
    addFeature(offset + 5, 10 * (last->x - 0.5));
    addFeature(offset + 6, 10 * (last->y - 0.5));
    
    // absolute degree
    addFeature(offset + 7, atan2(first->y - 0.5, first->x - 0.5));
    addFeature(offset + 8, atan2(last->y - 0.5, last->x - 0.5));
    
    // absolute distance
    addFeature(offset + 9, 10 * distance2(first));
    addFeature(offset + 10, 10 * distance2(last));
    
    // diff
    addFeature(offset + 11, 5 * (last->x - first->x));
    addFeature(offset + 12, 5 * (last->y - first->y));
}

float distance(const Node *n1, const Node *n2) {
    const float x = n1->x - n2->x;
    const float y = n1->y - n2->y;
    return sqrt(x * x + y * y);
}

float distance2(const Node *n1) {
    const float x = n1->x - 0.5;
    const float y = n1->y - 0.5;
    return sqrt(x * x + y * y);
}

void makeVertexFeature(int sid) {
    for (int i = 0; i < featurenodepairsIndex; ++i) {
        if (i > MAX_CHARACTER_SIZE) {
            break;
        }
        const Node *first = &(*(featurenodepairs + i)).first;
        const Node *last = &(*(featurenodepairs + i)).last;
        if (!first) {
            continue;
        }
        const int offset = sid * 1000 + 20 * i;
        makeBasicFeature(offset, first, last);
    }
}

void AddNotePare(int sid) {
    for (size_t i = 0; i < featurenodepairsIndex; ++i) {
        if (i > MAX_CHARACTER_SIZE) {
            break;
        }
        const Node *first = &(*(featurenodepairs + i)).first;
        const Node *last = &(*(featurenodepairs + i)).last;
        
        if (!first) {
            continue;
        }
        
        FeatureNodePair fp;
        fp.first = *first;
        fp.last = *last;
        featurenodepair_push_back(fp);
    }
}

void makeMoveFeature(int sid, const Node *first, const Node *last) {
    const int offset = 100000 + sid * 1000;
    makeBasicFeature(offset, first, last);
}

void feature_sort() {
    for (int i = 0; i < featurenodesIndex - 1; ++i) {
        for (int j = 0; j < featurenodesIndex - i - 1; ++j) {
            if ((*(featurenodes + j)).index > (*(featurenodes + j + 1)).index) {
                FeatureNode tmp = (*(featurenodes + j));
                (*(featurenodes + j)) = (*(featurenodes + j + 1));
                (*(featurenodes + j + 1)) = tmp;
            }
        }
    }
}

void classify() {
    int iNum = 0;
    int itemp1 = featurenodepairsIndex;
    int findDubblefirst = 0, findDoublelast = 0;
    
    FeatureNodePair *x = featurenodepairs;
    
    memset(resultnote, 0, PT_SIZE * sizeof(Resultnote));
    
    resultnote[0].iTimenum = -1;
    for (size_t i = 0; i < featurenodepairsIndex && i < PT_SIZE; ++i) {
        findDubblefirst = 0;
        findDoublelast = 0;
        //所有presultnote中的值与x.first进行比较，看是否重复
        for (size_t j = 0; j < iNum; ++j) {
            int ix = 0, iy = 0;
            ix = resultnote[j].iTimenum;
            iy = x[i].first.iTimeNum;
            if (resultnote[j].iTimenum == x[i].first.iTimeNum) {
                findDubblefirst = 1;
                break;
            }
            
            
        }
        //所有presultnote中的值与x.last进行比较，看是否重复
        for (size_t j = 0; j < iNum; ++j) {
            int ix = 0, iy = 0;
            ix = resultnote[j].iTimenum;
            iy = x[i].last.iTimeNum;
            if (resultnote[j].iTimenum == x[i].last.iTimeNum) {
                findDoublelast = 1;
                break;
            }
        }
        
        if (findDubblefirst == 0) {
            resultnote[iNum].x = x[i].first.x;
            resultnote[iNum].y = x[i].first.y;
            resultnote[iNum].iTimenum = x[i].first.iTimeNum;
            iNum++;
        }
        if (findDoublelast == 0) {
            resultnote[iNum].x = x[i].last.x;
            resultnote[iNum].y = x[i].last.y;
            resultnote[iNum].iTimenum = x[i].last.iTimeNum;
            iNum++;
        }
        
    }
    
    struct Resultnote iMinNote, iSwichNote;
    for (size_t i = 0; i < iNum; ++i) {
        iMinNote.x = resultnote[i].x;
        iMinNote.y = resultnote[i].y;
        itemp1 = resultnote[i].iTimenum;
        iMinNote.iTimenum = resultnote[i].iTimenum;
        
        
        for (size_t j = i + 1; j < iNum; ++j) {
            if (iMinNote.iTimenum < resultnote[j].iTimenum) continue;
            iSwichNote.x = iMinNote.x;
            iSwichNote.y = iMinNote.y;
            iSwichNote.iTimenum = iMinNote.iTimenum;
            
            iMinNote.x = resultnote[j].x;
            iMinNote.y = resultnote[j].y;
            itemp1 = resultnote[j].iTimenum;
            iMinNote.iTimenum = resultnote[j].iTimenum;
            
            resultnote[j].x = iSwichNote.x;
            resultnote[j].y = iSwichNote.y;
            resultnote[j].iTimenum = iSwichNote.iTimenum;
        }
        
        Result r;
        r.x = iMinNote.x;
        r.y = iMinNote.y;
        r.iTimeNum = iMinNote.iTimenum;
        r.iStorkeNum = (iMinNote.iTimenum) / 10000;
        if (r.x != 0 && r.y != 0)
            result_push_back(r);
    }
}
//***********************计算相似度************************
int characterCalSimilarityEx(char base[][CHARACTER_MAC_LEN], char* keystr, int liblen)
{
    int valueSimilarity = 0;
    for(int i=0; i<liblen; i++)
    {
        int similarity = 0;
        int loop = int(strlen((const char*)base[i]));
        int keylen = int(strlen((const char*) keystr));
        
        for(int j=0; j<loop; j++)
        {
            if(j > keylen)
                continue;
            if(*(base[i] + j) == *(keystr + j))
                similarity++;
        }
        if(i ==0)
        {
            valueSimilarity = similarity;
        }
        else
        {
            if(valueSimilarity < similarity)
                valueSimilarity = similarity;
        }
    }
    return valueSimilarity;
}
int characterCalSimilarity(int index, char* directionStr)
{
    switch (index) {
        case CHARACTER_ZERO:
            return characterCalSimilarityEx(zero, directionStr, ZERO_LIB_LEN);
        case CHARACTER_ONE:
            return characterCalSimilarityEx(one, directionStr, ONE_LIB_LEN);
        case CHARACTER_TWO:
            return characterCalSimilarityEx(two, directionStr, TWO_LIB_LEN);
        case CHARACTER_THREE:
            return characterCalSimilarityEx(three, directionStr, THREE_LIB_LEN);
        case CHARACTER_FOUR_1:
            return characterCalSimilarityEx(four_1, directionStr, FOUR_1_LIB_LEN);
        case CHARACTER_FIVE:
            return characterCalSimilarityEx(five, directionStr, FIVE_LIB_LEN);
        case CHARACTER_SIX:
            return characterCalSimilarityEx(six, directionStr, SIX_LIB_LEN);
        case CHARACTER_SEVEN:
            return characterCalSimilarityEx(seven, directionStr, SEVEN_LIB_LEN);
        case CHARACTER_EIGHT:
            return characterCalSimilarityEx(eight, directionStr, EIGHT_LIB_LEN);
        case CHARACTER_NINE:
            return characterCalSimilarityEx(nine, directionStr, NINE_LIB_LEN);
        case CHARACTER_RIGHT:
            return characterCalSimilarityEx(right, directionStr, RIGHT_LIB_LEN);
        case CHARACTER_WRONG_1:
            return characterCalSimilarityEx(wrong_1, directionStr, WRONG_1_LIB_LEN);
        case CHARACTER_HALF:
            return characterCalSimilarityEx(half, directionStr, HALF_LIB_LEN);
        case CHARACTER_POINT:
            return characterCalSimilarityEx(point, directionStr, POINT_LIB_LEN);
        case CHARACTER_FOUR_2:
            return characterCalSimilarityEx(four_2, directionStr, FOUR_2_LIB_LEN);
        case CHARACTER_WRONG_2:
            return characterCalSimilarityEx(wrong_2, directionStr, WRONG_2_LIB_LEN);
            
        default:
            break;
    }
    return 0;
}

int calSimilarity(char* directionStr)
{
    int retCharacter = 0;
    int valueSimilarity = 0;
    for(int i=0; i<CHARACTER_COUNT; i++)
    {
        if(i == CHARACTER_WRONG_1)
            continue;
        int ret = characterCalSimilarity(i, directionStr);
        if(i == 0)
            valueSimilarity = ret;
        else
        {
            if(valueSimilarity < ret)
            {
                retCharacter = i;
                valueSimilarity = ret;
            }
        }
    }
    return retCharacter;
}

//*************************  recognize number ***********************
float dot_distance(Dot *n1, Dot *n2) {
    const float x = n1->x - n2->x;
    const float y = n1->y - n2->y;
    return sqrt(x * x + y * y);
}

float dot_angle(Dot *n1, Dot *n2) {
    float height = n2->y - n1->y;
    float width = n1->x - n2->x;
    float rads = atan(height / width);
    return (180.0 * rads / PI);
}

bool contains(char base[][CHARACTER_MAC_LEN], char *keystr, int liblen) {
    bool ret = false;
    for (int j = 0; j < liblen; j++) {
        if (strcmp((const char *) base[j], (const char *) keystr) == 0) {
            ret = true;
            break;
        }
    }
    return ret;
}
bool equal(char* first, char* second) {
    if(strcmp((const char *)first, (const char *)second) == 0)
        return true;
    else
        return false;
}
int recognizeResult() {
    Dot dots[resultsIndex];
    
    for (int i = 0; i < resultsIndex; i++) {
        dots[i].x = results[i].x;
        dots[i].y = results[i].y;
    }
    
    // special point
    Dot startPoint = dots[0];
    Dot secondPoint = dots[1];
    Dot thirdPoint = dots[2];
    Dot endPoint = dots[resultsIndex - 1];
    Dot topPoint = dots[0];
    Dot bottomPoint = dots[0];
    Dot leftPoint = dots[0];
    Dot rightPoint = dots[0];
    // recongizer based points
    
    for (int i = 0; i < resultsIndex; i++) {
        dots[i].x = results[i].x;
        dots[i].y = results[i].y;
        
        if (dots[i].x < leftPoint.x) {
            leftPoint = dots[i];
        }
        if (dots[i].x > rightPoint.x) {
            rightPoint = dots[i];
        }
        if (dots[i].y < topPoint.y) {
            topPoint = dots[i];
        }
        if (dots[i].y > bottomPoint.y) {
            bottomPoint = dots[i];
        }
    }
    
    //  make directionstr
    memset(directionStr, 0, MAX_DIRECTIONSTR_CNT * sizeof(char));
    directionStrIndex = 0;
    
    directionStr[directionStrIndex++] = '0';
    for (int i = 0; i < resultsIndex; i++) {
        if (i) {
            // 取出相邻的两个点
            Dot point1 = dots[i - 1];
            Dot point2 = dots[i];
            float pointAngle = dot_angle(&point1, &point2);
            
            // 左上或右下
            if (pointAngle >= -90.0 && pointAngle <= 0.0) {
                
                // 左上
                if (
                    ((point1.x >= point2.x && point1.y > point2.y) ||
                     (point1.x > point2.x && point1.y >= point2.y)) &&
                    (directionStrIndex == 0 ||
                     (directionStrIndex > 0 && directionStr[directionStrIndex - 1] != '1'))
                    ) {
                    directionStr[directionStrIndex++] = '1';
                }
                
                // 右下
                if (((point1.x <= point2.x && point1.y < point2.y) ||
                     (point1.x < point2.x && point1.y <= point2.y)) &&
                    (directionStrIndex == 0 ||
                     (directionStrIndex > 0 && directionStr[directionStrIndex - 1] != '3'))
                    ) {
                    directionStr[directionStrIndex++] = '3';
                }
            }
            // 左下或右上
            if (pointAngle > 0.0 && pointAngle <= 90.0) {
                
                // 左下
                if ((point1.x >= point2.x && point1.y < point2.y) &&
                    (directionStrIndex == 0 ||
                     (directionStrIndex > 0 && directionStr[directionStrIndex - 1] != '2'))
                    ) {
                    directionStr[directionStrIndex++] = '2';
                }
                // 右上
                if (((point1.x < point2.x && point1.y >= point2.y) ||
                     (point1.x <= point2.x && point1.y > point2.y)) &&
                    (directionStrIndex == 0 ||
                     (directionStrIndex > 0 && directionStr[directionStrIndex - 1] != '4'))
                    ) {
                    directionStr[directionStrIndex++] = '4';
                }
            }
        }
    }
    
    if (contains(point, directionStr, POINT_LIB_LEN) &&
        dot_distance(&startPoint, &endPoint) <= 50) {
        
        return CHARACTER_POINT;
    }
    if (contains(zero, directionStr, ZERO_LIB_LEN) && dot_distance(&startPoint, &endPoint) <= 50) {
        return CHARACTER_ZERO;
    }
    if (contains(one, directionStr, ONE_LIB_LEN) && (dot_angle(&startPoint, &endPoint) <= -60 ||
                                                     dot_angle(&startPoint, &endPoint) > 60) && fabsf(float(rightPoint.x-startPoint.x)) < 30) {
        if (equal(directionStr, "032")) {
            if (secondPoint.y-startPoint.y < thirdPoint.y-secondPoint.y) {
                return CHARACTER_ONE;
            } else {
                return CHARACTER_FIVE;
            }
        }
        return CHARACTER_ONE;
    }
    if (contains(two, directionStr, TWO_LIB_LEN)) {
        if (equal(directionStr, "0423")) {
            if (resultsIndex < 5 || (resultsIndex == 5 && dot_angle(&secondPoint, &startPoint) < dot_angle(&secondPoint, &thirdPoint))) {
                return CHARACTER_TWO;
            } else {
                return CHARACTER_NINE;
            }
        }
        return CHARACTER_TWO;
    }
    if (contains(three, directionStr, THREE_LIB_LEN)) {
        if (equal(directionStr, "04232") || equal(directionStr, "04242")) {
            if (resultsIndex < 6 || (resultsIndex == 6 && dot_angle(&secondPoint, &startPoint) < dot_angle(&secondPoint, &thirdPoint))) {
                return CHARACTER_THREE;
            } else {
                return CHARACTER_NINE;
            }
        }
        return CHARACTER_THREE;
    }
    if (contains(four_1, directionStr, FOUR_1_LIB_LEN) && endPoint.x == bottomPoint.x &&
        endPoint.y == bottomPoint.y) {
        return CHARACTER_FOUR_1;
    }
    if (contains(four_2, directionStr, FOUR_2_LIB_LEN) && endPoint.y >= startPoint.y) {
        return CHARACTER_FOUR_2;
    }
    if (contains(five, directionStr, FIVE_LIB_LEN) && startPoint.x == topPoint.x &&
        startPoint.y == topPoint.y) {
        if (dot_angle(&startPoint, &secondPoint) > 30 || dot_angle(&startPoint, &secondPoint) < -45) {
            return CHARACTER_FIVE;
        }
    }
    if (contains(six, directionStr, SIX_LIB_LEN) && endPoint.y < bottomPoint.y) {
        return CHARACTER_SIX;
    }
    if (contains(seven, directionStr, SEVEN_LIB_LEN) && endPoint.x == bottomPoint.x &&
        endPoint.y == bottomPoint.y) {
        return CHARACTER_SEVEN;
    }
    if (contains(eight, directionStr, EIGHT_LIB_LEN)) {
        return CHARACTER_EIGHT;
    }
    if (contains(nine, directionStr, NINE_LIB_LEN)) {
        return CHARACTER_NINE;
    }
    if (contains(right, directionStr, RIGHT_LIB_LEN)) {
        return CHARACTER_RIGHT;
    }
    if (contains(half, directionStr, HALF_LIB_LEN)) {
        return CHARACTER_HALF;
    }
    if (contains(wrong_1, directionStr, WRONG_1_LIB_LEN)) {
        return CHARACTER_WRONG_1;
    }
    if (contains(wrong_2, directionStr, WRONG_2_LIB_LEN)) {
        return CHARACTER_WRONG_2;
    }
    return calSimilarity(directionStr);
}



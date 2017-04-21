# recognizer
1.此手势库专门针对阅卷打分
手势识别库：android端通过ndk打包成so库，java通过jni调用native识别方法：可以识别0-9，半对，错，对号，小数点。
2.具体native函数参考recognizer.java类里面的说明。
3.View层已做了连续输入支持，区分间隔为0.5s。
4.View层对手势库做了已知的容错处理。
5.Demo演示对半对参照满分为100，自由打分没有对其进行临界值判断。



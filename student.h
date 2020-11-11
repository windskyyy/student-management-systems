// 出生日期
struct Date {
	int year;
	int month;
	int day;
};

// 学生个人信息
struct Stu_info { 
	char sid[15]; // 学号 // e.g. 20171877138 可以通过学号的前四位判断年级
	char name[33]; // 姓名 3 ｜ 4个 汉字 utf8编码 3个字节一个汉字 最多可以支持 11 个汉字
	char cla[50]; // 计算机科学与技术学院金融大数据
};

// 学生档案管理
struct Stu_record : Stu_info {
	char gender[10]; // 性别 male | famale
	Date birth; // 出生日期
	char stufrom[90]; // 生源地 e.g. 山东省潍坊市昌乐县
	struct Stu_record *next;
	struct Stu_record *prev;
} initRecord;

// 学生成绩的链表
struct score_link {
	char course[50];
	double score;
	struct score_link *next;
} *tempScoreLink;

// 学生成绩管理
struct Stu_score : Stu_info {
	struct score_link *head;
	struct score_link *tail;
	double allScore;
	int courseCnt;
	struct Stu_score *next;
	struct Stu_score *prev;
} initScore;

struct Course {
	char cname[50]; // 课程名
	// char tname[50]; // 老师名
	// int maxContain; // 课程最多报名数
	struct Course *next;
	struct Course *prev;
} initCourse;

// 用于对成绩排序输出
struct node {
	double score;
	char sid[15];
	char name[50];
} scoreSort[100000];

// 桶排序
struct BucketNode {
	struct BucketNode *tail;
	struct node *nodepoint;
	struct BucketNode *next;
};

// 注册的管理员账号信息
struct AdminInfo {
	char account[50];
	char password[50];
	struct AdminInfo *next;
	// struct AdminInfo *tail; 
} *AdminInfoLink;


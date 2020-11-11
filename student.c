#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
// #include <iostream>
#include "student.h"
#include "hash.h"
#include "admin.h"


void inputStudentRecordByFile(char *file_name) ;
void inputStudentScoreByFile(char *file_name) ;
void inputCourseByFile(char *file_name) ;

// Linux 
// 	freopen("/dev/tty","r",stdint);
//	freopen("/dev/tty","w",stdout);

// window
//  freopen("CON","r",stdint);  
//  freopen("CON","w",stdout);  


// sptr = (struct data*)malloc(sizeof(struct data));
struct Stu_record *recordRoot = (struct Stu_record*) malloc (sizeof(struct Stu_record));
struct Stu_record *recordCur  = (struct Stu_record*) malloc (sizeof(struct Stu_record));
 
struct Stu_score *scoreRoot   = (struct Stu_score*) malloc (sizeof(struct Stu_score));
struct Stu_score *scoreCur    = (struct Stu_score*) malloc (sizeof(struct Stu_score));

struct Course *subjectRoot    = (struct Course*) malloc (sizeof(struct Course));
struct Course *subjectCur     = (struct Course*) malloc (sizeof(struct Course));

char checkBreak[5];

/*
struct tm
{
    int tm_sec;     // seconds after the minute - [0,59]
    int tm_min;     // minutes after the hour - [0,59] 
    int tm_hour;    // hours since midnight - [0,23] 
    int tm_mday;    // day of the month - [1,31] 
    int tm_mon;     // months since January - [0,11] 
    int tm_year;    // years since 1900 
    int tm_wday;    // days since Sunday - [0,6] 
    int tm_yday;    // days since January 1 - [0,365] 
    int tm_isdst;   // daylight savings time flag 
};

*/

char* getNowTimeString() {
	time_t nowtime = time(NULL);
	struct tm *local;
	local = localtime(&nowtime);
	char *ret = (char *) malloc (155);
	int len = 0;
	// year
	int t = local->tm_year+1900;
    ret[len++] = (t / 1000 + '0');
    ret[len++] = (t / 100 % 10 + '0');
    ret[len++] = (t % 100 / 10 + '0');
    ret[len++] = (t % 10 + '0');
    // month
    t = local->tm_mon + 1;
    if (t >= 10) {
    	ret[len++] = (t / 10 + '0');
    }  else {
    	ret[len++] = 0 + '0';
    }
    ret[len++] = (t % 10 + '0');
    // day
    t = local->tm_mday;
    if (t >= 10) {
    	ret[len++] = (t / 10 + '0');
    }  else {
    	ret[len++] = 0 + '0';
    }
    ret[len++] = (t % 10 + '0');
    ret[len++] = '_';
	// hour
	t = local->tm_hour;
	if (t >= 10) {
		ret[len++] = t / 10 + '0';
	} else {
		ret[len++] = 0 + '0';
	}
	ret[len++] = t % 10 + '0';
	ret[len++] = ':';
	// minute
	t = local->tm_min;
	if (t >= 10) {
		ret[len++] = t / 10 + '0';
	} else {
		ret[len++] = 0 + '0';
	}
	ret[len++] = t % 10 + '0';
	ret[len++] = ':';
	// second
	t = local->tm_sec;
	if (t >= 10) {
		ret[len++] = t / 10 + '0';
	} else {
		ret[len++] = 0 + '0';
	}
	ret[len++] = t % 10 + '0';
	// ret[len++] = '\t';
	ret[len] = '\0';
	// printf("hour = %d minute = %d second = %d\n", local->tm_hour, local->tm_min, local->tm_sec);// debug
	// printf("ret = %s\n", ret); //debug
	return ret;
}

// log文件，按天切割
void log(char *s) {
	char file_name[105] = "/Users/xutianmeng/Desktop/big_homework/log_";

	int len = strlen(file_name);
	time_t nowtime;
	nowtime = time(NULL); //获取日历时间  
    struct tm *local;
    local = localtime(&nowtime);
    int t = local->tm_year+1900;
    file_name[len++] = (t / 1000 + '0');
    file_name[len++] = (t / 100 % 10 + '0');
    file_name[len++] = (t % 100 / 10 + '0');
    file_name[len++] = (t % 10 + '0');

    t = local->tm_mon + 1;

    if (t >= 10) {
    	file_name[len++] = (t / 10 + '0');
    }  else {
    	file_name[len++] = 0 + '0';
    }
    file_name[len++] = (t % 10 + '0');

    t = local->tm_mday;
    if (t >= 10) {
    	file_name[len++] = (t / 10 + '0');
    }  else {
    	file_name[len++] = 0 + '0';
    }
    file_name[len++] = (t % 10 + '0');
    file_name[len] = '\0';
	
	FILE *fp = fopen(file_name, "a+");
	if (fp == nullptr) {
		puts("打开log文件失败");
		return ;
	}
	char *out = getNowTimeString();
	strcat(out, "\t");
	strcat(out, s);

	fprintf(fp, "%s\n", out);

	free(out); // 不释放会导致内存泄漏
	
	fclose(fp);
}

char transWord(char ch) {
	if (ch >= 'A' && ch <= 'Z') ch += 32;
	return ch;
}

bool judge(char* str) {
	// 支持随机大小写的end 
	if (strlen(str) == 3) {
		checkBreak[0] = transWord(str[0]);
		checkBreak[1] = transWord(str[1]);
		checkBreak[2] = transWord(str[2]);
		if (checkBreak[0] == 'e' && checkBreak[1] == 'n' && checkBreak[2] == 'd') return true;
	}
	return false;	
}

void saveRecord() {

	if (recordRoot->next == nullptr) { 
		log("当前record链表为空");
		return ;
	}

	struct Stu_record *t = (struct Stu_record*) malloc (sizeof(struct Stu_record));
	t = recordRoot->next;
	char *out = getNowTimeString();
	char s[100] = "/Users/xutianmeng/Desktop/big_homework/save_record_";
	strcat(s, out);

	FILE *fp = fopen(s, "a+");
	if (fp == nullptr) {
		log("打开record文件失败");
		return ;
	}

	while(t != nullptr) {
		fprintf(fp, "%s %s %s %d-%d-%d %s %s\n", t->sid, t->name, t->gender, t->birth.year, t->birth.month, t->birth.day, t->cla, t->stufrom);
		t = t->next;
	}
	fclose(fp);
	// freopen("/dev/tty","w",stdout);
	log("数据保存完成");
}

void saveScore() {
	
	if (scoreRoot->next == nullptr) {
		log("当前score链表为空");
		return ;
	}

	struct Stu_score *t = (struct Stu_score*) malloc (sizeof(struct Stu_score));
	t = scoreRoot->next;
	char *out = getNowTimeString();
	char s[100] = "/Users/xutianmeng/Desktop/big_homework/save_score_";
	strcat(s, out); // TODO
	
	FILE *fp = fopen(s, "a+");
	if (fp == nullptr) {
		log("打开score文件失败");
		return ;
	}
	while(t != nullptr) {
		tempScoreLink = t->head; // 这个链表居然是从head开始记录数据的，而不是head->next。 傻逼了。
		int cnt = 0;
		while (tempScoreLink != nullptr) {
			++cnt;
			tempScoreLink = tempScoreLink->next;
		}
		fprintf(fp, "%s %d\n", t->sid, cnt);

		tempScoreLink = t->head;
		while (tempScoreLink != nullptr) {
			fprintf(fp, "%s %.2f\n", tempScoreLink->course, tempScoreLink->score);
			tempScoreLink = tempScoreLink->next;
		}
		// fprintf(fp, "\n");
		t = t->next;
	}
	free(out);
	fclose(fp);
	// freopen("/dev/tty","w",stdout);

	log("数据保存完成\n");
}

void saveCourse() {

	if (subjectRoot->next == nullptr) {
		log("当前subject链表为空");
		return ;
	}

	struct Course *t = (struct Course*) malloc (sizeof(struct Course));
	t = subjectRoot->next;
	char *out = getNowTimeString();
	char s[100] = "/Users/xutianmeng/Desktop/big_homework/save_course_";
	strcat(s, out);
	
	FILE *fp = fopen(s, "a+");
	if (fp == nullptr) {
		log("打开course文件失败");
		return ;
	}
	while(t != nullptr) {
		fprintf(fp, "%s\n",t->cname);
		t = t->next;
	}
	fclose(fp);
	log("数据保存完成\n");
	// freopen("/dev/tty","w",stdout);
}
	int cnt = 0;



// coursepoint只存在于hash_course，op = 2的时候free（coursepoint）；
// scorepoint && recordpoint 都在hash_sid中有一份，所以op=2的时候free，op = 0的时候只置为null。
void dealClear(Hash *root, int op = 0) {

	struct Hash *cur = (struct Hash*) malloc (sizeof(struct Hash));
	cur = root->next;

	while (cur != nullptr) {

		if (cur->recordpoint != nullptr) {
			if (op == 1) free(cur->recordpoint);
			cur->recordpoint = nullptr;
		}

		if (cur->scorepoint != nullptr) {
			if (op == 1) {
				struct score_link *temp = (struct score_link *) malloc (sizeof(struct score_link));
				temp = cur->scorepoint->head;
				while (temp != nullptr) {
					free (temp);
					temp = temp->next;
				}
				free (cur->scorepoint);
			}
			cur->scorepoint = nullptr;
		}

		if (cur->coursepoint != nullptr) {
			if (op == 2) free(cur->coursepoint);
			cur->coursepoint = nullptr;
		}

		free(cur);
		cur = cur->next;
	}
}

// 清空全部数据
void clearAll() {
	for (int i = 0; i < 100005; i++) {
		dealClear(hash_sid[i].head, 1);
		dealClear(hash_name[i].head, 0);
		dealClear(hash_class[i].head, 0);
		dealClear(hash_course[i].head, 2);
	}
	log("清空所有数据完成");
}

// 从某个备份恢复数据，将当前的所有数据全部清空，重新插入文件的数据
void recoverAllData() {
	
	saveRecord();
	saveCourse();
	saveScore();
	puts("从某个备份恢复数据，将当前的所有数据全部清空，重新插入文件的数据");
	puts("分别输入档案文件，成绩文件，课程文件, 输入end取消恢复");
	char recordfile[1000], scorefile[1000], coursefile[1000];
	scanf("%s", recordfile);
	if (strcmp(recordfile, "end") == 0) return ;
	scanf("%s %s", scorefile, coursefile);

	clearAll();

	inputStudentRecordByFile(recordfile); 
	log("档案恢复完成");
	inputCourseByFile(coursefile);
	log("课程恢复完成");
	inputStudentScoreByFile(scorefile);
	log("成绩恢复完成");

	puts("恢复完成");
}

// 输入完整的学生档案
void inputStudentRecord() {
	saveRecord();
	puts("依次输入 学号，姓名，性别，出生日期，班级，生源地, 以end结尾");
	char sid[15], name[50], gender[10], cla[50], stufrom[100];
	Date birth;
	while(scanf("%s", sid) != EOF) {
		// 支持随机大小写的end 
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;
		scanf("%s %s %d-%d-%d %s %s", name, gender, &birth.year, &birth.month, &birth.day, cla, stufrom);

		struct Stu_record *temp = (struct Stu_record*) malloc (sizeof(struct Stu_record));

		if (select_recordHashBySid(sid) != nullptr) {
			puts("插入失败，学生档案已经存在了，请勿重复插入");
			continue;
		}

		puts("插入成功");

		strcpy(temp->sid, sid);	
		strcpy(temp->name, name); 
		strcpy(temp->gender, gender);
		strcpy(temp->cla, cla); 
		strcpy(temp->stufrom, stufrom);
		temp->birth = birth;


		// 超级record链表
		recordCur->next = temp;
		temp->prev = recordCur;
		recordCur = temp;
		recordCur->next = nullptr;

		if ( !insert_hash_record("sid", temp->sid, temp, temp->sid) ) {
			log("向hash_sid索引插入record失败");
		}
		if (!insert_hash_record("name", temp->name, temp, temp->sid)) {
 			log("向hash_name索引插入record失败");	
		}
		if ( !insert_hash_record("class", temp->cla, temp, temp->sid)) {
			log("向hash_class索引插入record失败");
		}
		
	}
	puts("本次插入学生档案信息操作完成");
}

// 使用文件插入档案数据
void inputStudentRecordByFile(char *file_name) {
	saveRecord();
	// puts("文件输入的格式为 学号 姓名 性别 出生日期(year-month-day) 班级 生源地");
	// char file_name[50];
	// while (true) {
		// puts("请输入文件的绝对路径, 以end结尾");
		// scanf("%s", file_name);
		// if (judge(file_name)) break;
		// FILE *fp = fopen("/Users/xutianmeng/Desktop/big_homework/inputRecordTest", "r+"); /Users/xutianmeng/Desktop/big_homework/inputScoreTest
		FILE *fp = fopen(file_name, "r+");
		char sid[15], name[50], gender[10], cla[50], stufrom[100];
		Date birth;
		while(fscanf(fp, "%s", sid) != EOF) {
			// 支持随机大小写的end 
			// if (judge(sid)) break;
			// if (strcmp(sid, "end") == 0) break;
			fscanf(fp, "%s %s %d-%d-%d %s %s", name, gender, &birth.year, &birth.month, &birth.day, cla, stufrom);
	
			struct Stu_record *temp = (struct Stu_record*) malloc (sizeof(struct Stu_record));
	
			if (select_recordHashBySid(sid) != nullptr) {
				log("已经存在了");
				continue;
			}
	// /Users/xutianmeng/Desktop/big_homework/inputRecordTest
			strcpy(temp->sid, sid);	
			strcpy(temp->name, name); 
			strcpy(temp->gender, gender);
			strcpy(temp->cla, cla); 
			strcpy(temp->stufrom, stufrom);
			temp->birth = birth;
	
	
			// 超级record链表
			recordCur->next = temp;
			temp->prev = recordCur;
			recordCur = temp;
			recordCur->next = nullptr;
	
			if ( !insert_hash_record("sid", temp->sid, temp, temp->sid) ) {
				log("向hash_sid索引插入record失败");
			}
			if (!insert_hash_record("name", temp->name, temp, temp->sid)) {
	 			log("向hash_name索引插入record失败");	
			}
			if ( !insert_hash_record("class", temp->cla, temp, temp->sid)) {
				log("向hash_class索引插入record失败");
			}
	
		}
		fclose(fp);
	// }
}

void test() {
	// 浅拷贝的问题
	puts("\n\n************ test ***************");
	int key = getHash("math");
	struct Hash *cur = (struct Hash*) malloc (sizeof(struct Hash));
	cur = hash_course[key].head;
	while (cur != nullptr) {
		printf("cur->key = %s \t cur->sid = %s \t\n\n", cur->key, cur->sid); // debug
		if (cur->scorepoint != nullptr) {
			puts("scorepoint is not null"); // debug
			printf("%s\n", cur->scorepoint->sid);
		}
		cur = cur->next;
	}
	puts("********************************\n");
}

// 根据学号录入课程+成绩
void inputStudentScore() {
	saveScore();
	puts("支持批量,请先输入学号，输入end表示结束");
	char sid[15];
	while(scanf("%s", sid) != EOF) {
		// test();
		// 支持随机大小写的end 
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;
		// test();
		struct Stu_record *temp = (struct Stu_record*) malloc (sizeof(struct Stu_record));
		temp = select_recordHashBySid(sid);
		if (temp == nullptr) { // not exist the student
			puts("输入有误，该学生不存在，可以在学生档案系统录入该生");
			continue;
		}
		struct Stu_score *t = (struct Stu_score*) malloc (sizeof(struct Stu_score));


		if (select_scoreHashBySid(sid) == nullptr) { // 如果在档案中但是不在score中，插入进去
			log("新插入一个学号");
			if (!insert_hash_score("sid", sid, t, sid)) {
				log("向hash_sid索引插入score失败"); // log
			} 
			t->head 	 = nullptr; // todo head是保存数据的，而不是head->next
			t->tail 	 = nullptr; // todo
			t->allScore  = 0;
			t->courseCnt = 0;
			// 超级score链表
			scoreCur->next = t;
			t->prev = scoreCur;
			t->next = nullptr;
			scoreCur = t;
		} else {
			t = select_scoreHashBySid(sid);
		}

		// 根据sid从record的hash表拿到数据，赋值给score表的指针，省去输入。
		strcpy(t->name, temp->name);
		strcpy(t->cla, temp->cla);
		strcpy(t->sid, sid);

		// // 超级score链表
		// scoreCur->next = t;
		// t->prev = scoreCur;
		// t->next = nullptr;
		// scoreCur = t;
		if (!insert_hash_score("class", t->cla, t, sid)) {
			log("向hash_class索引插入score失败"); // log
		}
		if (!insert_hash_score("name", t->name, t, sid)) {
			log("向hash_name索引插入score失败"); // log
		}
		char course[50];
		double score;
		puts("支持批量输入课程+成绩，输入end表示结束");
		// t->tail->next
		while(scanf("%s", course) != EOF) {
			// 支持随机大小写的end 
			if (judge(course)) break;

			scanf("%lf", &score);

			if (!exist_courseHashByCname(course)) {
				puts("插入失败，该课程不存在");
				continue;
			}

			bool isrepeat = false;
			// 判断是否重复插入
			struct score_link *tt = (struct score_link*) malloc (sizeof(struct score_link));
			tt = t->head;
			while (tt != nullptr) {
				if (strcmp(tt->course, course) == 0) {
					isrepeat = true;
					break;
				}
				tt = tt->next;
			}
			if (isrepeat == true) {
				puts("插入重复插入该门课程的成绩");
				continue;
			}

			struct score_link *tmp = (struct score_link*) malloc (sizeof(struct score_link));
			tmp->score = score;
			strcpy(tmp->course, course);
			tmp->next = nullptr;

			t->allScore += score;
			++t->courseCnt;

			if (t->head == nullptr && t->tail == nullptr) {
				t->head = tmp;
				t->tail = tmp;
			} else {
				t->tail->next = tmp;
				t->tail = tmp;
			}
			if (!insert_hash_score("course", course, t, sid)) {
				log("向hash_course索引插入score失败"); // log
			}

			// test();
			// break;
		} 
		puts("插入成功");
		// test();
	}
}

/**
文件输入格式：
	20171877138 1
	math 100
	20171877103 1
	math 60
*/
void inputStudentScoreByFile(char *file_name) {
	saveScore();
	FILE *fp = fopen(file_name, "r+");
	char sid[15];
	int cnt;
	while(fscanf(fp, "%s %d", sid, &cnt) != EOF) {
		// 支持随机大小写的end 
		// if (judge(sid)) break;
		struct Stu_record *temp = (struct Stu_record*) malloc (sizeof(struct Stu_record));
		temp = select_recordHashBySid(sid);
		if (temp == nullptr) { // not exist the student
			log("输入有误，该学生不存在，可以在学生档案系统录入该生");
			continue;
		}
		struct Stu_score *t = (struct Stu_score*) malloc (sizeof(struct Stu_score));


		if (select_scoreHashBySid(sid) == nullptr) { // 如果在档案中但是不在score中，插入进去
			log("新插入一个学号");
			if (!insert_hash_score("sid", sid, t, sid)) {
				log("向hash_sid索引插入score失败"); // log
			} 
			t->head 	 = nullptr; 
			t->tail 	 = nullptr;
			t->allScore  = 0;
			t->courseCnt = 0;
			// 超级score链表
			scoreCur->next = t;
			t->prev = scoreCur;
			t->next = nullptr;
			scoreCur = t;
		} else {
			t = select_scoreHashBySid(sid);
		}

		// 根据sid从record的hash表拿到数据，赋值给score表的指针，省去输入。 /Users/xutianmeng/Desktop/big_homework/inputScoreTest
		strcpy(t->name, temp->name);
		strcpy(t->cla, temp->cla);
		strcpy(t->sid, sid);

		if (!insert_hash_score("class", t->cla, t, sid)) {
			log("向hash_class索引插入score失败"); // log
		}
		if (!insert_hash_score("name", t->name, t, sid)) {
			log("向hash_name索引插入score失败"); // log
		}
		
		char course[50];
		double score;
		for (int i = 0; i < cnt; i++) {
			fscanf(fp, "%s %lf", course, &score);
			if (!exist_courseHashByCname(course)) {
				log("该课程不存在");
				continue;
			}
			struct score_link *tmp = (struct score_link*) malloc (sizeof(struct score_link));
			tmp->score = score;
			strcpy(tmp->course, course);
			tmp->next = nullptr;

			t->allScore += score;
			++t->courseCnt;

			if (t->head == nullptr && t->tail == nullptr) {
				t->head = tmp;
				t->tail = tmp;
			} else {
				t->tail->next = tmp;
				t->tail = tmp;
			}
			if (!insert_hash_score("course", course, t, sid)) {
				log("向hash_course索引插入score失败"); // log
			}
		}
	}
	fclose(fp);
}

// 输入课程名
void inputCourse() {
	saveCourse();
	puts("批量输入课程名称, 以end为结尾");
	char cname[30];
	while(scanf("%s", cname) != EOF) {
		struct Course *temp = (struct Course*) malloc (sizeof(struct Course)); // 
		// 支持随机大小写的end 
		if (judge(cname)) break;
		// if (strcmp(cname, "end") == 0) break;
		strcpy(temp->cname, cname);

		if (!insert_hash_course("course", cname, temp)) {
			log("向hash_course索引插入course失败"); // log
		}

		// 超级course链表
		subjectCur->next = temp;
		temp->prev = subjectCur;
		temp->next = nullptr;
		subjectCur = temp;
	}
}

void inputCourseByFile(char *file_name) {
	saveCourse();
	// puts("批量输入课程名称, 以end为结尾");
	char cname[300];
	FILE *fp = fopen(file_name, "r+"); 
	while(fscanf(fp, "%s", cname) != EOF) {
		struct Course *temp = (struct Course*) malloc (sizeof(struct Course)); // 
		// 支持随机大小写的end 
		// if (judge(cname)) break;
		// if (strcmp(cname, "end") == 0) break;
		strcpy(temp->cname, cname);
		
		if (!insert_hash_course("course", cname, temp)) {
			log("向hash_course索引插入course失败"); // log
		}

		// 超级course链表
		subjectCur->next = temp;
		temp->prev = subjectCur;
		temp->next = nullptr;
		subjectCur = temp;
	}
	fclose(fp);
}

// 根据学号查询该学生的档案信息
void queryRecordHashInfoBySid() {
	char sid[15];
	while (true) {
		puts("支持批量查询sid， 以end结尾");
		scanf("%s", sid);
		// 支持随机大小写的end 
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;

		struct Stu_record *info = (struct Stu_record*) malloc (sizeof(struct Stu_record));
		info = select_recordHashBySid(sid);
		if (info == nullptr) {
			puts("该学生还没有插入过档案");
			continue;
		}
		printf("%s %s %s %04d-%02d-%02d %s %s\n",info->sid, info->name, info->gender, 
					info->birth.year, info->birth.month, info->birth.day, 
					info->cla, info->stufrom); // 精确查询
	}
}

// 根据学号查询该学生的成绩信息
void queryScoreHashInfoBySid() {
	char sid[15];
	while (true) {
		puts("支持批量查询sid， 以end结尾");
		scanf("%s", sid);
		// 支持随机大小写的end 
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;

		struct Stu_score *info = (struct Stu_score*) malloc (sizeof(struct Stu_score));
		info = select_scoreHashBySid(sid);
		if (info == nullptr) {
			puts("该学生还没有插入过成绩");
			continue;
		}
		struct score_link *tempLink = (struct score_link*) malloc (sizeof(struct score_link));
		printf("%s %s %s\n", info->sid, info->name, info->cla);
		tempLink = info->head;
		while(tempLink != nullptr) {
			printf("%s %.2f\n", tempLink->course, tempLink->score);
			tempLink = tempLink->next;
		}
	}
}

// 根据班级查询该班级的学生信息
void queryClassInfoByClassName() {
	char ClassName[50];
	while (true) {
		puts("支持批量查询，输入班级名称，查询该班级的学生档案or学生成绩信息， 以end结尾");
		scanf("%s", ClassName);
		// 支持随机大小写的end 
		if (judge(ClassName)) break;
		// if (strcmp(ClassName, "end") == 0) break;
		while (true) {
			puts("1->查询学生档案 0->查询学生成绩, 其他表示退出");
			int op = 0;
			scanf("%d",&op);
			if (op != 0 && op != 1) break;
			select_classHashByClass(ClassName, op);
		}
	}
}

// 根据姓名查询record的hash表
void queryStudentHashInfoByName() {
	char name[50];
	while (true) {
		puts("支持批量查询，输入名字，以end结尾");
		scanf("%s", name);
		// 支持随机大小写的end 
		if (judge(name)) break;
		// if (strcmp(name, "end") == 0) break;
		puts("1->查询学生档案 2->查询学生成绩");
		int op = 1;
		scanf("%d", &op);
		if (op == 1) select_recordHashByName(name);
		else select_scoreHashByName(name);
	}
}

// 查询某门课程的信息，目前只支持判断课程是否存在
void queryCouserInfoByCname() {
	char cname[50];
	while (true) {
		puts("支持批量查询，输入课程名称，以end结尾");
		scanf("%s", cname);
		// 支持随机大小写的end 
		if (judge(cname)) break;
		// if (strcmp(cname, "end") == 0) break;
		exist_courseHashByCname(cname);
	}
}

// 查询某个班级的男女比例
void queryMenAndWomenPercentageByClass() {
	char ClassName[50];
	while (true) {
		puts("支持批量查询，输入班级名称，以end结尾");
		scanf("%s", ClassName);
		// 支持随机大小写的end 
		if (judge(ClassName)) break;
		// if (strcmp(ClassName, "end") == 0) break;
		select_MenAndWomenPercentageByClass(ClassName);
	}
}

// 查询某门课程的成绩分布
void queryCourseScoreDistributionByCname() {
	char cname[50];
	while (true) {
		puts("支持批量查询，输入课程名称，以end结尾");
		scanf("%s", cname);
		// 支持随机大小写的end 
		if (judge(cname)) break;
		// if (strcmp(cname, "end") == 0) break;
		select_CourseScoreDistributionByCourse(cname);
	}
}

// 查询某门课程的学生成绩，按照成绩排序输出，可以选择输出前n名
void queryCourseScoreLimitByCourse() {
	char cname[50];
	int desc = 0;
	int limit ;
	while (true) {
		puts("批量查询，输入课程名，升序输入0，降序输入1， 是否需要指定前n个人,不需要输入0，以end结尾");
		scanf("%s", cname);
		// 支持随机大小写的end 
		if (judge(cname)) break;
		// if (strcmp(cname, "end") == 0) break;
		scanf("%d %d", &desc, &limit);
		if (limit == 0) limit = -1;
		select_CourseScoreByCourse(cname, desc, limit);
	}
}

// 根据学号删除学生的所有信息 [档案信息+成绩信息+课程的成绩]
void deleteStudentInfoBySid() {
	saveRecord();
	saveCourse();
	saveScore();
	char sid[15];
	char last[5];
	while (true) {
		puts("批量查询，输入学号，删除此学生的所有信息, 以end结尾");
		scanf("%s", sid);
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;
		puts("真的要删除吗？确认请输入y");
		scanf("%s", last);
		if (last[0] >= 'A' && last[0] <= 'Z') last[0] += 32;
		if (last[0] != 'y') {
			puts("取消删除");
			continue;
		}
		deleteStudentInfoBySid(sid);
		puts("删除成功");
	}
}

// 根据学号修改学生档案信息
void changeStudentRecordBySid() {
	saveRecord();
	alterStudentRecordBySid();
}

// 根据课程为单位，修改课程成绩
void changeStudentScoreByCourseName() {
	saveScore();
	char course[50];
	while (true) {
		puts("支持批量查询，输入课程名称，然后输入学生学号和修改后的成绩, 以end为结尾");
		scanf("%s", course);
		if (judge(course)) break;
		// if (strcmp(course, "end") == 0) break;
		alterStudentScoreByCourse(course);
	}
}

// 计算程序效率
void calc() {
	clock_t startTime,endTime;
	startTime = clock();
	endTime = clock();
	printf("%f\n",double(endTime-startTime)/CLOCKS_PER_SEC);
}

void init() {
	initHash();

	recordRoot->next  = nullptr;
	recordRoot->prev  = nullptr;
	recordCur         = recordRoot;

	scoreRoot->next   = nullptr;
	scoreRoot->prev   = nullptr;
	scoreRoot->head   = nullptr;
	scoreRoot->tail   = nullptr;
	scoreCur 		  = scoreRoot;

	subjectRoot->next = nullptr;
	subjectRoot->prev = nullptr;
	subjectCur 		  = subjectRoot;

}

void inputmenu() {

	int op = 0;
	char filename[50];
	while (1) {
		puts("\n=============插入数据=============");
		puts("1 从命令行插入学生档案信息");
		puts("2 从命令行插入学生成绩信息");
		puts("3 从命令行插入课程信息");
		puts("4 从文件插入学生档案信息");
		puts("5 从文件插入学生成绩信息");
		puts("6 从文件插入课程信息");
		// puts("退出请输入0");
		puts("=============退出请输入0==========\n");
		
		scanf("%d", &op);
		if (op == 0) break;

		switch (op) {
			case 1:
				inputStudentRecord();
				break;
			case 2:
				inputStudentScore();
				break;
			case 3:
				inputCourse();
				break;
			case 4:
				puts("请输入文件的绝对路径");
				scanf("%s", filename);
				inputStudentRecordByFile(filename);
				break;
			case 5:
				puts("请输入文件的绝对路径");
				scanf("%s", filename);
				inputStudentScoreByFile(filename);
				break;
			case 6:
				puts("请输入文件的绝对路径");
				scanf("%s", filename);
				inputCourseByFile(filename);
				break;
		}
	}
}

void querymenu() {
	int op;
	while (1) {
		puts("\n=============查询数据=============");
		puts("1 查询某门课程的信息");
		puts("2 根据姓名查询档案信息");
		puts("3 查询某个班级的男女比例");
		puts("4 查询某门课程的成绩分布");
		puts("5 查询某门课程的学生成绩");
		puts("6 根据学号查询该学生的档案信息");
		puts("7 根据学号查询该学生的成绩信息");
		puts("8 根据班级查询该班级的学生信息");
		// puts("退出请输入0");
		puts("=============退出请输入0==========\n");
		scanf("%d", &op);
		if (op == 0) break;

		switch (op) {
			case 1:
				queryCouserInfoByCname();
				break;
			case 2:
				queryStudentHashInfoByName();
				break;
			case 3:
				queryMenAndWomenPercentageByClass();
				break;
			case 4:
				queryCourseScoreDistributionByCname();
				break;
			case 5:
				queryCourseScoreLimitByCourse();
				break;
			case 6:
				queryRecordHashInfoBySid();
				break;
			case 7:
				queryScoreHashInfoBySid();
				break;
			case 8:
				queryClassInfoByClassName();
				break;
		}
	}
}

void altermenu() {
	int op;
	while (1) {
		puts("\n==========修改数据============");
		puts("1. 根据学号修改学生档案信息");
		puts("2. 根据课程为单位，修改课程成绩");
		puts("==========退出请输入0=========\n");
		scanf("%d",&op);
		if (op == 0) break;
		switch (op) {
			case 1:
				changeStudentRecordBySid();
				break;
			case 2:
				changeStudentScoreByCourseName();
				break;
		}
	}
}
void adminmenu() {
	int op;
	while (1) {
		puts("\n=========管理员信息=========");
		puts("1. 注册管理员账号");
		puts("2. 删除管理员账号");
		puts("3. 查询管理员账号");
		// puts("退出请输入0"); 
		puts("=========退出请输入0========\n");
		scanf("%d", &op);
		if (op == 0) break;

		switch (op) {
			case 1:
				registerAdmin();
				break;
			case 2:
				deleteAdmin();
				break;
			case 3:
				showAdmin();
		}
	}
}

int main() {

	int adminId = logincheck();
	if (adminId == 0) return 0;

	init();
	int op1;
	while (1) {
		puts("\n     学生管理系统");
		puts("*************************");
		puts("1. 插入数据");
		puts("2. 查询数据");
		puts("3. 删除数据");
		puts("4. 修改数据");
		puts("5. 恢复数据");
		puts("6. 退出当前用户");
		if (adminId == 1) {
			puts("7. 超级管理员系统");
		}
		puts("退出请输入0");
		puts("*************************\n");
		scanf("%d", &op1);
		switch (op1) {
			case 0:
				return 0;
			case 1:
				inputmenu();
				break;
			case 2:
				querymenu();
				break;
			case 3:
				deleteStudentInfoBySid();
				break;
			case 4:
				altermenu();
				break;
			case 5:
				recoverAllData();
				break;
			case 6:
				adminId = logincheck();
				if (adminId == 0) return 0;
				break;
			case 7:
				adminmenu();
				break;
		}
	}

	return 0;
	
		
	

	return 0;
}

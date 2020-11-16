extern void log(char *s);
extern char transWord(char ch);
extern bool judge(char* str);
// extern checkBreak[5];

// Hash结构
struct Hash {
	struct Hash *head; // 指向该桶的第一个
	struct Hash *tail; // 指向该桶的最后一个
	struct Hash *prev; // 双向链表
	struct Hash *next;
	struct Stu_record *recordpoint; // 记录record类型指针
	struct Stu_score *scorepoint;   // 记录score类型指针
	struct Course *coursepoint;     // 记录course类型指针
	char key[50]; // char *key					// 记录下来键值的原本样子，所有的键值都是字符串类型。
	char sid[20]; // char *sid						// 主键 学号
} hash_name[100005], hash_sid[100005], hash_class[100005], hash_course[100005];


// 将hash表中的指针初始化
void initHash() {
	for(int i = 0; i < 100005; i++) {
		hash_name[i].head = &hash_name[i];
		hash_name[i].tail = &hash_name[i];
		hash_name[i].prev = nullptr;
		hash_name[i].next = nullptr;
		hash_name[i].recordpoint = nullptr;
		hash_name[i].scorepoint = nullptr;
		hash_name[i].coursepoint = nullptr;

		hash_sid[i].head = &hash_sid[i];
		hash_sid[i].tail = &hash_sid[i];
		hash_sid[i].prev = nullptr;
		hash_sid[i].next = nullptr;
		hash_sid[i].recordpoint = nullptr;
		hash_sid[i].scorepoint = nullptr;
		hash_sid[i].coursepoint = nullptr;


		hash_class[i].head = &hash_class[i];
		hash_class[i].tail = &hash_class[i];
		hash_class[i].prev = nullptr;
		hash_class[i].next = nullptr;
		hash_class[i].recordpoint = nullptr;
		hash_class[i].scorepoint = nullptr;
		hash_class[i].coursepoint = nullptr;

		hash_course[i].head = &hash_course[i];
		hash_course[i].tail = &hash_course[i];
		hash_course[i].prev = nullptr;
		hash_course[i].next = nullptr;
		hash_course[i].recordpoint = nullptr;
		hash_course[i].scorepoint = nullptr;
		hash_course[i].coursepoint = nullptr;
		// strcpy(hash_course[i].key,"123");
		// strcpy(hash_course[i].sid,"123");
	}
}

// 哈希算法
unsigned int BKDRHash(char *str) {
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

// 链式实现，取余降低大小，但是会造成更多的冲突，有可能导致哈希链表长度过长。
int getHash(char *str) {
	return BKDRHash(str) % 100003;
}

// 快速排序
void quicksort(int left, int right, struct node *a) {
 	if (left >= right) return ;

	struct node temp = a[left];
	double score = a[left].score;

	int i = left, j = right;
	while (i < j) {
		while (i < j && a[j].score >= score) --j;
		a[i] = a[j];

		while (i < j && a[i].score <= score) ++i;
		a[j] = a[i];
	}
	a[i] = temp;
	if (i - 1 > left) quicksort(left, i-1, a);
	if (right >i + 1) quicksort(i+1, right, a);
}

// 桶排序
void bucketsort(struct node *a, int len) {
	BucketNode bucket[1000];
	for(int i = 0; i < 1000; i++) {
		bucket[i].next = nullptr;
		bucket[i].tail = nullptr;
		bucket[i].nodepoint = nullptr;
	}
	for(int i = 0; i < len; i++) {
		int score = (int)a[i].score;
		if (bucket[score].next == nullptr) { // 第一次插入
			struct BucketNode *tempbucket = (struct BucketNode*) malloc (sizeof(struct BucketNode));
			tempbucket->next = nullptr;
			bucket[score].next = tempbucket;
			bucket[score].tail = tempbucket;
			struct node *tempnode = (struct node*) malloc (sizeof(struct node));
			tempnode->score = a[i].score;
			strcpy(tempnode->sid, a[i].sid);
			strcpy(tempnode->name, a[i].name);
			tempbucket->nodepoint = tempnode;
		} else {
			struct BucketNode *tempbucket = (struct BucketNode*) malloc (sizeof(struct BucketNode));
			tempbucket->next = nullptr;
			bucket[score].tail->next = tempbucket;
			bucket[score].tail = tempbucket;
			struct node *tempnode = (struct node*) malloc (sizeof(struct node));
			tempnode->score = a[i].score;
			strcpy(tempnode->sid, a[i].sid);
			strcpy(tempnode->name, a[i].name);
			tempbucket->nodepoint = tempnode;
		}
	}
	int nowpos = 0;
	struct BucketNode *tempbucket;
	for(int i = 0; i < 1000; i++) {
		tempbucket = bucket[i].next;
		while (tempbucket != nullptr) {
			a[nowpos].score = tempbucket->nodepoint->score;
			strcpy(a[nowpos].name, tempbucket->nodepoint->name);
			strcpy(a[nowpos].sid, tempbucket->nodepoint->sid);
			++nowpos;
			tempbucket = tempbucket->next;
		}
	}
}


// 插入档案的hash表, 先判断是否已经存在了。
bool insert_recordHash(Hash *head, char *str, struct Stu_record *point, char *sid) {
	struct Hash *cur = head->next;
	while (cur != nullptr) {
		if (strcmp(str, cur->key) == 0 && strcmp(cur->sid, sid) == 0/* && cur->recordpoint != nullptr*/) {
			// log(sprintf("冲突的学号 = %s\n", sid));
			if (cur->recordpoint != nullptr) {
				log("正在覆盖索引的recordpoint,停止插入"); // debug
				return false;
			} else {
				log("找到了该学生的信息，只添加新的recordpoint");
			}
			cur->recordpoint = point;
			return false;
		}
		cur = cur->next;
	}	

	cur = head->tail;
	struct Hash *newHash = (struct Hash*) malloc (sizeof(struct Hash));
	// newHash->sid 		 = sid;
	strcpy(newHash->sid, sid);
	newHash->prev 		 = cur;	
	// newHash->key 		 = str;
	strcpy(newHash->key, str);
	newHash->next 		 = nullptr;
	newHash->recordpoint = point;

	cur->next = newHash;
	head->tail = newHash;

	
	return true;
}

// 插入成绩的hash表, 先判断是否已经存在了。
bool insert_scoreHash(Hash *head, char *str, struct Stu_score *point, char *sid) {
	// return true;
	struct Hash *cur = head->next;
	while (cur != nullptr) {
		// printf("\n\ncur->key = %s \t cur->sid = %s \t sid = %s \t str = %s \n\n", cur->key, cur->sid, sid, str); // debug
		// if (cur->scorepoint != nullptr) {
		// 	puts("scorepoint is not null"); // debug
		// 	printf("%s\n\n", cur->scorepoint->sid);
		// }
		if (strcmp(str, cur->key) == 0 && strcmp(cur->sid, sid) == 0/* && cur->scorepoint != nullptr*/) { 
		//  scorepoint不能被覆盖！因为他有一个课程和成绩的指针，那个也会丢失。！！！！！！
			if (cur->scorepoint != nullptr) {
				log("正在覆盖索引的scorepoint,停止插入"); // debug
				return false;
			} else {
				log("找到了该学生的信息，只添加新的scorepoint");
			}
			cur->scorepoint = point;
			// log(sprintf("cur->key = %s, str = %s, cur->sid = %s, sid = %s, cur->scorepoint = %d\n", cur->key, str, cur->sid, sid, 
			// 														 	   cur->scorepoint == nullptr ? 0 : 1));
			return true; // 找到了sid已经插入的位置，只是可能已经有scorepoint或者没有了。
		}
		cur = cur->next;
	}
	log("没有找到任何信息，新插入一个结点"); //debug
	cur = head->tail;
	struct Hash *newHash = (struct Hash*) malloc (sizeof(struct Hash));
	// newHash->sid 		= sid;
	strcpy(newHash->sid, sid);
	newHash->prev 		= cur;	
	strcpy(newHash->key, str);
	// newHash->key 		= str;
	newHash->next  		= nullptr;
	newHash->scorepoint = point;
	cur->next = newHash;
	head->tail = newHash;

	// printf("新插入的结点信息: newHash->sid=%s,newHash->key=%s,newHash->scorepoint->sid=%s\n\n", newHash->sid, newHash->key,newHash->scorepoint->sid);

	return true; // 没找到sid在此hash表中的位置，新插入的。
}

// 插入课程的hash表, 先判断是否已经存在了。
// 传进来Hash指针的head
bool insert_courseHash(Hash *head, char *str, struct Course *point, char *sid = "") {
	struct Hash *cur = head->next;

	while (cur != nullptr) {
		if (strcmp(str, cur->key) == 0 && cur->coursepoint != nullptr) {
			return false;
		}
		cur = cur->next;
	}
	cur = head->tail;
	struct Hash *newHash = (struct Hash*) malloc (sizeof(struct Hash));
	newHash->prev        = cur;	
	// newHash->sid         = sid;
	// newHash->key         = str;
	strcpy(newHash->sid, sid);
	strcpy(newHash->key, str);
	newHash->next        = nullptr;
	newHash->coursepoint = point;

	cur->next = newHash;
	head->tail = newHash;

	return true;
}

// insert_hash系列的函数的作用是将[record | score | course]的指针插入到 [sid | name | course | class] 的hash表中！！！！！

/**
 * 向record的hash表插入数据，学号、姓名、班级为键值。
 * 插入成功返回true 否则false
 */
bool insert_hash_record(char *op, char *str, struct Stu_record *point, char *sid) {
	int key = getHash(str);
	if (strcmp(op, "sid") == 0) {
		return insert_recordHash(hash_sid[key].head, str, point, sid);
	} else if (strcmp(op, "name") == 0) {
		return insert_recordHash(hash_name[key].head, str, point, sid);
	} else if (strcmp(op, "class") == 0) {
		return insert_recordHash(hash_class[key].head, str, point, sid);
	} else {
		puts("index insert error"); // todo
		return false;
	}
}

// 向course的hash表插入数据，课程名为键值。插入成功返回true 否则false
bool insert_hash_course(char *op, char *str, struct Course *point, char *sid = "") {
	int key = getHash(str);
	if (strcmp(op, "course") == 0) {
		return insert_courseHash(hash_course[key].head, str, point);
	} else if (strcmp(op, "name") == 0) {
		return insert_courseHash(hash_name[key].head, str, point, sid);
	}
	else {
		puts("index insert error"); // todo
		return false;
	}
}

// 向score的hash表插入数据，可以根据sid为key。插入成功返回true 否则false
bool insert_hash_score(char *op, char *str, struct Stu_score *point, char *sid) {
	int key = getHash(str);
	if (strcmp(op, "sid") == 0) {
		return insert_scoreHash(hash_sid[key].head, str, point, sid);
	} else if (strcmp(op, "name") == 0) {
		return insert_scoreHash(hash_name[key].head, str, point, sid);
	} else if (strcmp(op, "class") == 0) {
		return insert_scoreHash(hash_class[key].head, str, point, sid);
	} else if (strcmp(op, "course") == 0) { // 某门课程的学生成绩信息
		return insert_scoreHash(hash_course[key].head, str, point, sid);
	}
	return false;
}

// 根据学号查询record的hash表
struct Stu_record* select_recordHashBySid(char *sid) {
	int key = getHash(sid);
	struct Hash *temp = hash_sid[key].head->next;
	while(temp != nullptr) {
		if (strcmp(temp->key, sid) == 0 && strcmp(temp->sid, sid) == 0 && temp->recordpoint != nullptr) {
			return temp->recordpoint;
		}
		temp = temp->next;
	}
	return nullptr;
}

// 根据学号查询score的hash表
struct Stu_score* select_scoreHashBySid(char *sid) {
	int key = getHash(sid);
	struct Hash *temp = hash_sid[key].head->next;

	while(temp != nullptr) {
		if (strcmp(temp->key, sid) == 0 && strcmp(temp->sid, sid) == 0 && temp->scorepoint != nullptr) {
			return temp->scorepoint;
		}
		temp = temp->next;
	} 
	return nullptr;
}

// 根据姓名查询record的hash表, 直接输出，省事儿～
void select_recordHashByName(char *name) {
	int key = getHash(name);
	struct Hash *temp = hash_name[key].head->next;

	if (temp == nullptr) {
		puts("没有这个学生");
		return ;
	}

	while (temp != nullptr) {
		if (strcmp(temp->key, name) == 0 && temp->recordpoint != nullptr) {
			printf("%s %s %s %04d-%02d-%02d %s %s\n",temp->recordpoint->sid, temp->recordpoint->name, temp->recordpoint->gender, 
					temp->recordpoint->birth.year, temp->recordpoint->birth.month, temp->recordpoint->birth.day, 
					temp->recordpoint->cla, temp->recordpoint->stufrom); // 精确查询
		}
		temp = temp->next;
	}
}

// 根据姓名查询score的hash表，直接输出所有数据 todo 调整输出的格式 更加好看一些～
void select_scoreHashByName(char *name) {
	int key = getHash(name);
	struct Hash *temp = hash_name[key].head->next;

	if (temp == nullptr) { // 说明是空的，没有这个键值
		puts("没有这个学生");
		return ;
	}

	while(temp != nullptr) {
		struct score_link *tempLink;
		if (strcmp(temp->key, name) == 0 && temp->scorepoint != nullptr) {
			printf("%s %s %s\n", temp->scorepoint->sid, temp->scorepoint->name, temp->scorepoint->cla);
			// tempLink = temp->scorepoint->head->next;
			tempLink = temp->scorepoint->head; // todo ? maybe
			while(tempLink != nullptr) {
				printf("%s %.2f\n", tempLink->course, tempLink->score);
				tempLink = tempLink->next;
			}
		} 
		temp = temp->next;
	}
}
// todo 整理格式
// 根据班级查询该班级的学生信息，可选两种方式
void select_classHashByClass(char *cla, bool isRecord) {
	int key = getHash(cla);
	struct Hash *temp = hash_class[key].head->next;

	if (temp == nullptr) { // 说明是空的，没有这个键值
		puts("没有这个班级 或者 班级为空");
		return ;
	}
	while (temp != nullptr) {
		if (strcmp(temp->key, cla) == 0) {
			// 这里可以查询该班级的学生档案信息 or 学生成绩信息
			if (isRecord == 1) { // 学生档案信息
				if (temp->recordpoint == nullptr) {
					puts("该生没有档案");
					temp = temp->next;
					continue;
				}
				printf("%s %s %s %04d-%02d-%02d %s %s\n",temp->recordpoint->sid, temp->recordpoint->name, temp->recordpoint->gender, 
					temp->recordpoint->birth.year, temp->recordpoint->birth.month, temp->recordpoint->birth.day, 
					temp->recordpoint->cla, temp->recordpoint->stufrom);
			} else { // 学生成绩信息
				if (temp->scorepoint == nullptr) {
					puts("该生没有成绩");
					temp = temp->next;
					continue;
				}
				struct score_link *tempLink;
				printf("%s %s %s\n", temp->scorepoint->sid, temp->scorepoint->name, temp->scorepoint->cla);
				tempLink = temp->scorepoint->head;
				while(tempLink != nullptr) {
					printf("%s %.2f\n", tempLink->course, tempLink->score);
					tempLink = tempLink->next;
				}
			}
		}
		temp = temp->next;
	}
}

// 根据课程名判断是否存在
bool exist_courseHashByCname(char *cname) {
	int key = getHash(cname);
	return hash_course[key].head->next == nullptr ? false : true;
}

// 查询某个班级的男女比例
void select_MenAndWomenPercentageByClass(char *cla) {
	int key = getHash(cla);
	struct Hash *temp = hash_class[key].head->next;

	if (temp == nullptr) { // 说明是空的，没有这个键值
		puts("没有这个班级 或者 班级为空");
		return ;
	}

	double menCnt = 0, womenCnt = 0;
	while (temp != nullptr) {
		if (strcmp(temp->key, cla) == 0) {
			if (temp->recordpoint != nullptr && strcmp(temp->recordpoint->gender, "male") == 0) {
				++menCnt;
			} else if (temp->recordpoint != nullptr && strcmp(temp->recordpoint->gender, "female") == 0) {
				++womenCnt;
			}
		}
		temp = temp->next;
	}
	double allCnt = menCnt + womenCnt;
	printf("班级一共 %.0f人， 男生 %.0f人， 占比 %.2f%%， 女生 %.0f人,  占比 %.2f%%， \n", allCnt, menCnt, (menCnt/allCnt)*100, womenCnt, (womenCnt/allCnt)*100);
}

// 查询某门课程的成绩分布
void select_CourseScoreDistributionByCourse(char *course) {
	int key = getHash(course);
	struct Hash *temp = hash_course[key].head->next;

	if (temp == nullptr) { // 说明是空的，没有这个键值
		puts("没有这个课程");
		return ;
	}

	double minScore = 2e9, maxScore = -2e9;
	int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0, cnt = 0;

	while (temp != nullptr) {
		if (strcmp(temp->key, course) == 0 && temp->scorepoint != nullptr) {
			struct score_link *scorelink = temp->scorepoint->head; // todo mk mistake
			if (scorelink == nullptr) {
				log("这个学生没有任何课程成绩\n");
				continue;
			}
			while (scorelink != nullptr) {
				if (strcmp(scorelink->course, course) == 0) {
					minScore = minScore > scorelink->score ? scorelink->score : minScore;
					maxScore = maxScore < scorelink->score ? scorelink->score : maxScore;
					if (scorelink->score >= 90) {
						++cnt1;
					} else if (scorelink->score >= 80) {
						++cnt2;
					} else if (scorelink->score >= 70) {
						++cnt3;
					} else if (scorelink->score >= 60) {
						++cnt4;
					} else {
						++cnt5;
					}
					++cnt;
					break; // 一个学生之可能选一次这门课
				}
				scorelink = scorelink->next;
			}
		}
		temp = temp->next;
	}
	if (maxScore == -2e9) {
		puts("没有一个人");
		return ;
	}
	printf("最高分 = %.2f 最低分 = %.2f\n", maxScore, minScore);
	printf("一共有 %d人， 90分及以上有%d人， 80～89有%d人，70～79有%d人， 60～69有%d人， 有%d人不及格\n", cnt, cnt1, cnt2, cnt3, cnt4, cnt5);
}

// 查询某门课程的学生成绩，按照成绩排序输出，可以选择输出前n名
void select_CourseScoreByCourse(char *course, bool desc,  int limit) {

	int key = getHash(course);
	struct Hash *temp = hash_course[key].head->next;

	if (temp == nullptr) { // 说明是空的，没有这个键值
		puts("没有这个课程");
		return ;
	}

	memset(scoreSort, 0 ,sizeof scoreSort);
	int top = 0;

	while (temp != nullptr) {
		if (strcmp(temp->key, course) == 0 && temp->scorepoint != nullptr) {
			struct score_link *scorelink = temp->scorepoint->head; 
			if (scorelink == nullptr) {
				puts("这个学生没有任何课程成绩");// log
				continue;
			} 
			while (scorelink != nullptr) {
				if (strcmp(scorelink->course, course) == 0) {
					strcpy(scoreSort[top].sid,temp->scorepoint->sid);
					strcpy(scoreSort[top].name,temp->scorepoint->name);
					scoreSort[top].score = scorelink->score;
					++top;
					break;
				}
				scorelink = scorelink->next;
			}
		}
		temp = temp->next;
	}
	quicksort(0, top-1, scoreSort);
	if (desc == true) {
		int l = 0, r = top - 1;
		while (l < r) {
			struct node tt = scoreSort[l];
			scoreSort[l] = scoreSort[r];
			scoreSort[r] = tt;
			++l, --r;
		}
	} 
	if (limit == -1) limit = top;
	// limit = top;
	limit = limit > top ? top : limit;
	for(int i = 0; i < limit; i++) {
		printf("sid = %s name = %s score = %.2f\n", scoreSort[i].sid, scoreSort[i].name, scoreSort[i].score);
	}	
}

// 从score的链表中拿到课程名，然后去课程hash表里删掉学生的信息。
bool deleteStduentCourseInfoByCourse(char *cname, char *sid) {
	int key = getHash(cname);
	struct Hash *temp = hash_course[key].head->next;

	if (temp == nullptr) {
		return false;
	}

	while (temp != nullptr) {
		if (strcmp(temp->sid, sid) == 0) {
			temp->prev->next = temp->next;
			if (temp->next != nullptr) temp->next->prev = temp->prev;
			// free(temp); // 是否会造成内存泄漏
			if (temp->coursepoint) {
				temp->coursepoint->prev->next = temp->coursepoint->next;
				if (temp->coursepoint->next != nullptr) temp->coursepoint->next->prev = temp->coursepoint->prev;
			}
			break;
		}
		temp = temp->next;
	}
	return true;
}

// 根据学号删除学生的所有信息 档案信息+成绩信息
bool deleteStudentInfoBySid(char *sid) {
	// 先判断是否存在
	int keySid = getHash(sid);

	struct Hash *temp = hash_sid[keySid].head->next;

	if (temp == nullptr) {
		puts("没有此学生");
		return false;
	}

	char name[50];
	char cla[50];

	// 从 hash_sid ->  record + score 表里拿到数据 然后释放掉
	while (temp != nullptr) {
		if (strcmp(temp->sid, sid) == 0) {
			if (temp->recordpoint != nullptr) {
				strcpy(name, temp->recordpoint->name);
				strcpy(cla, temp->recordpoint->cla);
				temp->recordpoint->prev->next = temp->recordpoint->next;
				if (temp->recordpoint->next != nullptr) temp->recordpoint->next->prev = temp->recordpoint->prev;
			}
			if (temp->scorepoint != nullptr && temp->scorepoint->head != nullptr) {
				struct score_link *templink = temp->scorepoint->head;
				while (templink != nullptr) {
					deleteStduentCourseInfoByCourse(templink->course, sid);
					templink = templink->next;
				}
				temp->scorepoint->prev->next = temp->scorepoint->next;
				if (temp->scorepoint->next != nullptr) temp->scorepoint->next->prev = temp->scorepoint->prev;
			}
			temp->prev->next = temp->next;
			if (temp->next != nullptr) temp->next->prev = temp->prev;
			// free(temp);
			break;
		}
		temp = temp->next;
	}
	// 开始删除hash_name表中的数据, 重名的用sid筛选
	int keyName = getHash(name);
	temp = hash_name[keyName].head->next;

	if (temp == nullptr) {
		puts("没有此学生"); //  todo log 
	}

	while (temp != nullptr) {
		if (strcmp(temp->sid, sid) == 0) {
			temp->prev->next = temp->next;
			if (temp->next != nullptr) temp->next->prev = temp->prev;
			// free(temp);
			break;
		}
		temp = temp->next;
	}

	// 开始删除hash_class表中的数据
	int keyClass = getHash(cla);
	temp = hash_class[keyClass].head->next;

	if (temp == nullptr) {
		puts("没有此学生"); //  todo log 
	}

	while (temp != nullptr) {
		if (strcmp(temp->sid, sid) == 0) {
			temp->prev->next = temp->next;
			if (temp->next != nullptr) temp->next->prev = temp->prev;
			// free(temp);
			break;
		}
		temp = temp->next;
	}
	return true;
}

struct Hash* exist_studentBySid(char *sid) {
	int key = getHash(sid);
	struct Hash *temp = hash_sid[key].head->next;

	while (temp != nullptr) {
		if (strcmp(sid, temp->sid) == 0){
			return temp;
		}
		temp = temp->next;
	}
	return nullptr;
}

// 根据学号修改学生档案信息
void alterStudentRecordBySid() {
	char sid[15], name[50], gender[10], cla[50], stufrom[50];
	Date birth;
	struct Hash *temp;
	struct Hash *t;
	while (true) {
		puts("支持批量修改，输入学号，以end为结尾");
		scanf("%s", sid);
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;
		temp = exist_studentBySid(sid);
		if (temp == nullptr) {
			printf("没有这个学生 sid = %s\n", sid);
			continue;
		}
		char op[50];
		puts("输入想修改的内容，e.g. 姓名 徐天梦 或者这些性别，班级，生日，生源地, end结尾");
		while (true) {
			scanf("%s", op);
			if (judge(op)) break;
			// if (strcmp(op, "end") == 0) break;
			if (strcmp(op, "姓名") == 0) { // 需要修改索引列,还要修改scorepoint的数据 
				scanf ("%s", name);
				int key = getHash(temp->recordpoint->name);	 // 通过学号拿到了学生姓名，然后去hash_name找到。
				t = hash_name[key].head->next;
				while (t != nullptr) {
					if (strcmp(sid, t->sid) == 0) {
						int newKey = getHash(name);
						hash_name[newKey].tail->next = t;
						t->prev->next = t->next;
						t->next->prev = t->prev;
						t->prev = hash_name[newKey].tail;
						t->next = nullptr;
						strcpy(t->key, name);
						// t->key = name;
						hash_name[newKey].tail = t;						
						break;
					}
					t = t->next;
				}
				strcpy(temp->recordpoint->name, name);
				strcpy(temp->scorepoint->name, name);
			} else if (strcmp(op, "性别") == 0) {
				scanf("%s" ,gender);
				strcpy(temp->recordpoint->gender, gender);
			} else if (strcmp(op, "班级") == 0) {  // 需要修改索引列,还要修改scorepoint的数据
				scanf("%s", cla);
				int key = getHash(temp->recordpoint->cla);
				t = hash_class[key].head->next;
				while (t != nullptr) {
					if (strcmp(sid, t->sid) == 0) {
						int newKey = getHash(cla);
						hash_class[newKey].tail->next = t;
						t->prev->next = t->next;
						t->next->prev = t->prev;
						t->prev = hash_name[newKey].tail;
						t->next = nullptr;
						strcpy(t->key, cla);
						// t->key = cla;
						hash_class[newKey].tail = t;	
					}
					t = t->next;
				}
				strcpy(temp->recordpoint->cla, cla);
				strcpy(temp->scorepoint->cla, cla);
			} else if (strcmp(op, "生日") == 0) {
				scanf("%d-%d-%d",&birth.year, &birth.month, &birth.day);
				temp->recordpoint->birth = birth;
			} else if (strcmp(op, "生源地") == 0) {
				scanf("%s", stufrom);
				strcpy(temp->recordpoint->stufrom, stufrom);
			} 
		}
	}
}

// 根据课程为单位，修改课程成绩
// TODO ？？？？忘了todo什么了
void alterStudentScoreByCourse(char *course) {
	char sid[15];
	double score;
	struct Hash *temp;
	while (true) {
		puts("支持批量输入，输入学生的学号 成绩进行修改，以end为结尾");
		scanf("%s", sid);
		if (judge(sid)) break;
		// if (strcmp(sid, "end") == 0) break;
		scanf("%lf", &score);
		temp = exist_studentBySid(sid);

		if (temp == nullptr) {
			puts("没有这个学生");
			continue;
		}

		if (temp->scorepoint == nullptr) {
			log("scorepoint为nullptr"); // log
		}

		struct score_link *temp_scorelink = temp->scorepoint->head;

		while (temp_scorelink != nullptr) {
			if (strcmp(temp_scorelink->course, course) == 0) {
				temp_scorelink->score = score;
				puts("修改成功");
				break;
			}
			temp_scorelink = temp_scorelink->next;
		}
		
		if (temp_scorelink == nullptr) {
			puts("修改失败，在该学生的数据中没找到该门课程");
		}

	}
}

// 修改课程名和班级名没实现，原因是如果出现了hash冲突，会导致一个槽出现不同课程的学生，导致混乱难以记录。 todo 其实也可做，加课程名字段就ok，懒了。





void insertAdmin(char *account, char *password);

// 超级ROOT用户账号密码
char accountROOT[50] = "cxl";
char passwordROOT[50] = "cxl";
bool isFirstLogin = true;

const char adminFile[50] = "/Users/xutianmeng/Desktop/big_homework/save_admin";

// 管理员登陆
int logincheck() {
	char loginaccount[50];
	char loginpassword[50];
	// 先读入之前记录的账号密码
	if (isFirstLogin == true) {
		isFirstLogin = false;
		FILE *fp = fopen(adminFile, "r");
		while (fscanf(fp, "%s %s", loginaccount, loginpassword) != EOF) {
			insertAdmin(loginaccount,loginpassword);
		}
		fclose(fp);
	}
	while (true) {
		puts("请输入管理员账号密码, 退出请输入end");
		scanf("%s", loginaccount);
		if (judge(loginaccount)) return 0;
		scanf("%s", loginpassword);
		if (strcmp(loginaccount,accountROOT) == 0 && strcmp(loginpassword, passwordROOT) == 0) {
			puts("超级管理员登陆， 验证成功");
			return 1;
		}
		struct AdminInfo *temp = AdminInfoLink;
		while (temp != nullptr) {
			if (strcmp(loginaccount,temp->account) == 0 && strcmp(loginpassword, temp->password) == 0) {
				puts("普通管理员登陆， 验证成功");
				return 2;
			}
			temp = temp->next;
		}
		puts("账号 or 密码输入错误，请重试！");
	}
}

// 判断是否重复
bool existAdmin(char *account) {
	if (strcmp(account, accountROOT) == 0) return true;
	struct AdminInfo *temp = AdminInfoLink;
	while (temp != nullptr) {
		if (strcmp(account,temp->account) == 0) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void insertAdmin(char *account, char *password) {
	struct AdminInfo *temp = (struct AdminInfo*) malloc (sizeof(struct AdminInfo));
	strcpy(temp->account, account);
	strcpy(temp->password, password);
	temp->next = nullptr;

	if (AdminInfoLink == nullptr) {
		AdminInfoLink = temp;
	} else {
		temp->next = AdminInfoLink->next;
		AdminInfoLink->next = temp;
	}
}

// 注册普通管理员账号 只有超级管理员有权限，普通管理员登陆的时候不展示或者提示permission deined
// 还需要判断重复
void registerAdmin() {
	char account[50];
	char password[50];
	FILE *fp = fopen(adminFile, "a+");
	while (true) {
		puts("请输入账号密码， 支持批量输入账号，输入end表示结束");
		scanf("%s", account);
		if (judge(account)) break;
		scanf("%s", password);

		if (existAdmin(account) == true) {
			puts("账号重复了，请重新输入");
			continue;
		}
		insertAdmin(account, password);
		fprintf(fp, "%s %s\n", account, password);
		puts("账号注册成功");
	}
	fclose(fp);
}

// 删除普通管理员账号 只有超级管理员有权限，普通管理员登陆的时候不展示或者提示permission deined
void deleteAdmin() {
	char account[50];
	while (true) {
		puts("请输入账号， 支持批量输入账号，输入end表示结束");
		scanf("%s", account);
		if (judge(account)) break;

		if (!existAdmin(account)) {
			puts("账号不存在");
			continue;
		}
		if (strcmp(account, accountROOT) == 0) {
			puts("超级管理员账号不可删除");
			continue;
		}
		struct AdminInfo *temp = (struct AdminInfo*) malloc (sizeof(struct AdminInfo));
		if (strcmp(AdminInfoLink->account, account) == 0) {
			AdminInfoLink = AdminInfoLink->next;
			puts("删除成功");
		} else {
			temp = AdminInfoLink;
			while (temp != nullptr && temp->next != nullptr) {
				if (strcmp(temp->next->account, account) == 0) {
					struct AdminInfo *q = temp->next;
					temp->next = temp->next->next;
					free(q);
					puts("删除成功");
					break;
				}
				temp = temp->next;
			}
		}
	}
	FILE *fp = fopen(adminFile, "w");
	struct AdminInfo *t = AdminInfoLink;
	while (t != nullptr) {
		fprintf(fp, "%s %s\n", t->account, t->password);
		t = t->next;
	}
	fclose(fp);
}

void showAdmin() {
	struct AdminInfo *temp = AdminInfoLink;
	int number = 0;
	bool first = 1;
	while (temp != nullptr) {
		if (first) {
			first = 0;
			puts("管理员账号密码:");
		}
		printf("%d. account = %s password = %s\n", ++number, temp->account, temp->password);
		temp = temp->next;
	}
}
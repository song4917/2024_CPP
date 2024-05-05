#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <string>

struct Student {
    std::string name;
    int score1;
    int score2;
};

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int* scores = reinterpret_cast<int*>(NotUsed);
    scores[0] += atoi(argv[1]); // 과목 1의 총점
    scores[1] += atoi(argv[2]); // 과목 2의 총점
    scores[2]++; // 학생 수
    std::cout << "학생 이름: " << argv[1] << ", 과목 1: " << argv[2] << ", 과목 2: " << argv[3] << std::endl;
    int total = atoi(argv[2]) + atoi(argv[3]);
    std::cout << "총점: " << total << ", 평균: " << (total / 2.0) << std::endl;
    return 0;
}

int main() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("sj.db", &db);

    if (rc) {
        std::cerr << "DB 열기 실패: " << sqlite3_errmsg(db) << std::endl;
        return(0);
    }
    else {
        std::cout << "DB 열기 성공" << std::endl;
    }

    const char* sql = "SELECT * FROM student;";
    int scores[3] = { 0, 0, 0 }; // 과목별 총점과 학생 수를 저장할 배열

    rc = sqlite3_exec(db, sql, callback, scores, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL 에러: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "과목 1 총점: " << scores[0] << ", 평균: " << (scores[0] / (float)scores[2]) << std::endl;
        std::cout << "과목 2 총점: " << scores[1] << ", 평균: " << (scores[1] / (float)scores[2]) << std::endl;
    }

    sqlite3_close(db);

    return 0;
}

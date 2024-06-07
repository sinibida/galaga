#ifndef SCORE_H // 중복 include 방지
#define SCORE_H

// 준하: galaga.ino의 int score와 충돌: 이름 변경(score -> scoreboard)
namespace scoreboard
{                                          // 최종 점수, 최고 점수(BEST)
  unsigned int scoreRecord[3] = {0, 0, 0}; // 최고 점수 기록 (초깃값 : 0)
  extern unsigned int finalScore = 0;      // 최종 점수 <--- 이 변수를 메인에서 사용해서
  // 최종 점수를 저장
  // unsigned int readFinalScore(unsigned int scores) {
  //   // 최종 점수를 읽어오는 함수
  //   finalScore = scores;
  //   return finalScore;
  // }//게임 루틴이 끝나면어떻게 받아오게요? 게임 루틴 끝나도 값은 finalScore 변수에 저장되어 있으니까 그것만 출력하면 상관없지 않을까요 아 저걸 직접 메인에서사용해서?
  // // 스코어를 정렬해서 저장
  // 혹시 모르니 주석 처리만
  void scoreSort()
  { // 최고 점수 정렬 및 저장
    // 점수 정렬 일반적인 경우(1,0,0)에서 1보다 큰 수가 들어올 경우
    // 점수가 같은 경우도 포함 (ex. 최종 : 2 2 1)

    if (scoreRecord[0] <= finalScore)
    {
      scoreRecord[2] = scoreRecord[1];
      scoreRecord[1] = scoreRecord[0];
      scoreRecord[0] = finalScore;
    }
    else if (scoreRecord[1] <= finalScore)
    {
      scoreRecord[2] = scoreRecord[1];
      scoreRecord[1] = finalScore;
    }
    else if (scoreRecord[2] <= finalScore)
    {
      scoreRecord[2] = finalScore;
    }
  }

  void getScore(bool isBoss)
  { // 적 처치 시 점수 부여
    if (isBoss)
    {
      finalScore += 50;
    }
    else
    {
      finalScore += 1;
    }
  }

  void resetScore() {
    finalScore = 0;
  }
}; // namespace scoreboard
// 출력은 graphic.h에 구현

#endif
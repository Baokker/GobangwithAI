#pragma once

constexpr int BLACK = 1;
constexpr int WHITE = -1;
constexpr int EMPTY = 0;
constexpr int DRAW = 2;
constexpr int BOARDSIZE = 15;
const int NOWINNER = 3;

//AI
const int AI = 0;
const int PLAYER = 1;
const int DEPTH = 10;

//����
const int BE_FIVE = 10000000;
//����
const int ACTIVIE_FOUR = 50000;
//����
const int RUSH_FOUR = 3000;
//����
const int ACTIVIE_THREE = 3000;
//����
const int SLEEP_THREE = 200;
//���
const int ACTIVE_TWO = 200;
//�߶�
const int SLEEP_TWO = 10;
//����
const int OTHER = 1;
const int updown = 0; //���·���
const int leftright = 1; //���ҷ���
const int leftoblique = 2;//��б��
const int rightoblique = 3;//��б��

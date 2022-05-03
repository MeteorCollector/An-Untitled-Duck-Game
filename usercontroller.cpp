#include "usercontroller.h"
#include <transformbuilder.h>
#include <grid.h>
#include <bomb.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <messagebox.h>

UserController::UserController() {}



void UserController::onAttach() {
    if (playerID == 2 || playerID == 3)
    {
        pl1 = map->player1->getComponent<UserController>();
        assert(pl1 != nullptr);
        pl2 = map->player2->getComponent<UserController>();
        assert(pl2 != nullptr);
    }
    physics = this->gameObject->getComponent<Physics>();
    assert(physics != nullptr);
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    trans = this->gameObject->getComponent<Transform>();
    assert(trans != nullptr);
    assert(map != nullptr);
    //auto circle = new QGraphicsEllipseItem(trans);
    //circle->setRect(QRectF(-22, 0, 44, 32));
    //this->collider = circle;

    // health bar
    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(- totalhealth * 3, -40, totalhealth * 6, 6));
    Rect->setBrush(Qt::black);
    Bar = new QGraphicsRectItem(trans);
    Bar->setRect(QRectF(- totalhealth * 3, -40, health * 6, 6));
    Bar->setBrush(Qt::red);

    // bufftag
    tag = new QGraphicsTextItem(trans);
    tag->setPos(36, -48);
    auto bfont = new QFont();
    bfont->setPixelSize(10);
    bfont->setBold(true);
    tag->setDefaultTextColor(QColor(0, 0, 0));
    tag->setFont(*bfont);

    // label
    label = new QGraphicsTextItem(trans);
    label->setPos(-40, -70);
    if(playerID == 0)
    {
        label->setDefaultTextColor(QColor(0, 255, 0));
        playerName = "Player 1";
    }
    else if(playerID == 1)
    {
        label->setDefaultTextColor(QColor(0, 0, 255));
        playerName = "Player 2";
    }
    else if(playerID == 2)
    {
        label->setDefaultTextColor(QColor(255, 0, 0));
        playerName = "Robot  1";
    }
    else if(playerID == 3)
    {
        label->setDefaultTextColor(QColor(255, 0, 0));
        playerName = "Robot  2";
    }
    label->setPlainText(playerName);
    labelUpdate();

    auto font = new QFont();
    font->setPixelSize(20);
    font->setBold(true);
    label->setFont(*font);
    //text->setBrush(QBrush(Qt::green));
    //auto labeltrans = new Transform();
    //labeltrans->setPos(trans->pos()+QPointF(0, -20));
    //auto label = new GameObject();

    effect = new QSoundEffect();
    effect->setVolume(100.0f);

}

void UserController::say(QString str)
{
    auto msb = new MessageBox();
    auto obj = new GameObject();
    auto mtrans = new Transform();
    mtrans->setPos(trans->pos());
    msb->str = str;
    obj->addComponent(mtrans);
    obj->addComponent(msb);
    map->gms->attachGameObject(obj);
}

void UserController::harm(int damage)
{
    if(!isAlive) { return; }
    health = health - damage < 0 ? 0 : health - damage;
    Bar->setRect(QRectF(- totalhealth * 3, -40, health * 6, 6));
    effect->stop();
    if (playerID == 0 || playerID == 1) effect->setSource(QUrl("qrc:/pr/audios/classic_hurt.wav"));
    if (playerID == 2 || playerID == 3) effect->setSource(QUrl("qrc:/pr/audios/hurt2.wav"));
    effect->play();
    say("被炸到了！好疼啊...");
    if(health <= 0){ return die(); }
}

void UserController::die()
{
    physics->setVelocity(0, 0);// 停止！
    isAlive = false;
    imgtrans->setImage(":/player/images/tomb.png");
    auto p1 = map->player1->getComponent<UserController>();
    auto p2 = map->player2->getComponent<UserController>();
    if(!map->pvpEnabled)
    {
        auto r1 = map->robot1->getComponent<UserController>();
        auto r2 = map->robot2->getComponent<UserController>();
        if(!r1->isAlive && !r2->isAlive && (p1->isAlive || p2->isAlive))
        {
            map->mplr->stop();
            return map->victoryUI(p1->score + p2->score);
        }
        if(!p1->isAlive && !p2->isAlive && (r1->isAlive || r2->isAlive))
        {
            map->mplr->stop();
            return map->deathUI(p1->score + p2->score);
        }
    }
    if(p1->isAlive && !p2->isAlive && (map->pvpEnabled))
    {
        map->mplr->stop();
        return map->pvpEndUI("Player 1", p1->score);
    }
    if(!p1->isAlive && p2->isAlive && (map->pvpEnabled))
    {
        map->mplr->stop();
        return map->pvpEndUI("Player 2", p2->score);
    }
}

void UserController::labelUpdate()
{
    label->setPlainText(playerName + " score = " + QString::number(score));//
}

void UserController::onUpdate(float deltaTime) {

    myTimer += deltaTime;
    calmTime -= deltaTime;
    if (!isAlive) { return; }

    float vx = 0, vy = 0;
    int pace = 8;// 每pace步切换一次画面
    int x = trans->pos().x();
    int y = trans->pos().y();
    /*
        four-point judgement
        point1(-w, 0)   point2(w, 0)
        point3(0, b)  point4(0, h)
    */

    int b = 4, h = 24, w = 16;
    i = (y - 64 + 0 + 24) / 48;
    j = (x - 92 + 0 + 32) / 64;

    //if(moving && (playerID == 2 || playerID == 3)){ return; }// 这里myTimer可以设置setMove在机器人静止时的调用频率

    int i1 = (y - 64 + 0 + 24) / 48;//y1
    int j1 = (x - 92 - w + 32) / 64;//x1
    int i2 = (y - 64 + 0 + 24) / 48;//y2
    int j2 = (x - 92 + w + 32) / 64;//x2
    int i3 = (y - 64 + b + 24) / 48;//y3
    int j3 = (x - 92 + 0 + 32) / 64;//x3
    int i4 = (y - 64 + h + 24) / 48;//y4
    int j4 = (x - 92 + 0 + 32) / 64;//x4
    //Rect->setRect(QRectF(- w, b, 2 * w, h - b));

    //u_en = i > 0 && !(map->tile[i - 1][j] && (y < 64 + 48 * i - 22));// classic
    //d_en = i < 14 && !(map->tile[i + 1][j] && (y > 64 + 48 * i - 18));
    //l_en = j > 0 && !(map->tile[i][j - 1] && (x < 92 + 64 * j - 10));
    //r_en = j < 19 && !(map->tile[i][j + 1] && (x > 92 + 64 * j + 10));

    if(map->tile[i][j] < 0)// deal with buff
    {
        auto grd = map->arr[i][j]->getComponent<Grid>();
        int index = - map->tile[i][j];
        if(index >= 1){
            score += 100;
            labelUpdate();
            effect->stop(); effect->setSource(QUrl("qrc:/pr/audios/levelup.wav")); effect->play();
            bstr[2] = bstr[1]; bstr[1] = bstr[0];
        }
        if(index == 1){ velocity = 200; say("吃到加速道具了！"); buffcount[0]++; buff.push(1); bstr[0] = "SpeedUp "; }
        else if(index == 2){ bombGrade = 4; say("炸弹威力提升了！"); buffcount[1]++; buff.push(2); bstr[0] = "UpgradedBomb "; }
        else if(index == 3){ bombSum = 8; say("可以放置更多炸弹了"); buffcount[2]++; buff.push(3); bstr[0] = "MoreBomb "; }
        else if(index == 4){ bombMove = true; say("现在可以踢炸弹了！"); buffcount[3]++; buff.push(4); bstr[0] = "KickBomb "; }
        destory(grd->stf);
        map->tile[i][j] = 0;
        tag->setPlainText(bstr[0] + bstr[1] + bstr[2]);
        if(buff.size() > 3){ //最多同时吃三种状态
            int otb = buff.front(); buff.pop(); buffcount[otb - 1]--;
            if(otb == 1 && buffcount[0] <= 0){ velocity = 100; say("                              ...但是速度降低了"); }
            else if(otb == 2 && buffcount[1] <= 0){ bombGrade = 1; say("                              ...但是炸弹威力下降了"); }
            else if(otb == 3 && buffcount[2] <= 0){ bombSum = 1; say("                              ...但是只能放一个炸弹了"); }
            else if(otb == 4 && buffcount[3] <= 0){ bombMove = false; say("                              ...但是不能踢炸弹了"); }
        }
    }

    u_en = i > 0 && !((map->tile[i1 - 1][j1] > 0 || map->tile[i2 - 1][j2] > 0) && (y < 64 + 48 * i - 20));
    d_en = i < 14 && !((map->tile[i1 + 1][j1] > 0 || map->tile[i2 + 1][j2] > 0) && (y > 64 + 48 * i - 18));
    l_en = j > 0 && !((map->tile[i3][j3 - 1] > 0 || map->tile[i4][j4 - 1] > 0) && (x < 92 + 64 * j - 10));
    r_en = j < 19 && !((map->tile[i3][j3 + 1] > 0 || map->tile[i4][j4 + 1] > 0) && (x > 92 + 64 * j + 10));

    if (playerID == 2 || playerID == 3)
    {
        if (calmTime > 0) { myTimer = 0; moving = 0; }
        if (myTimer > loopTime)
        {
            myTimer = 0;
            return map->putBomb(i, j, bombGrade, 3, playerID);
        }
        //qDebug("detecting movement of robot ", 1);//
        if (moving == 0) return setMove(false);
        if (moving > 0)
        {
            if(moving == 1)// 1 - up; 2 - down; 3 - left; 4 - right
            {
                if(y < targety){ trans->setPos(targetx, targety); moving = 0; }
                vy -= velocity;
                y /= pace; y %= 8;
                switch(y){
                case 0:
                    imgtrans->setImage(":/robot/images/rbt_u_1.png");
                    break;
                case 1:
                    imgtrans->setImage(":/robot/images/rbt_u_2.png");
                    break;
                case 2:
                    imgtrans->setImage(":/robot/images/rbt_u_3.png");
                    break;
                case 3:
                    imgtrans->setImage(":/robot/images/rbt_u_4.png");
                    break;
                case 4:
                    imgtrans->setImage(":/robot/images/rbt_u_5.png");
                    break;
                case 5:
                    imgtrans->setImage(":/robot/images/rbt_u_6.png");
                    break;
                case 6:
                    imgtrans->setImage(":/robot/images/rbt_u_7.png");
                    break;
                case 7:
                    imgtrans->setImage(":/robot/images/rbt_u_8.png");
                    break;
                default:
                    imgtrans->setImage(":/robot/images/rbt_u_3.png");
                    break;
                }
                trans->moveBy(0, - deltaTime * velocity);
            }
            else if(moving == 2)
            {
                if(y > targety){ trans->setPos(targetx, targety); moving = 0; }
                y /= pace; y %= 8;
                switch(y){
                case 0:
                    imgtrans->setImage(":/robot/images/rbt_d_1.png");
                    break;
                case 1:
                    imgtrans->setImage(":/robot/images/rbt_d_2.png");
                    break;
                case 2:
                    imgtrans->setImage(":/robot/images/rbt_d_3.png");
                    break;
                case 3:
                    imgtrans->setImage(":/robot/images/rbt_d_4.png");
                    break;
                case 4:
                    imgtrans->setImage(":/robot/images/rbt_d_5.png");
                    break;
                case 5:
                    imgtrans->setImage(":/robot/images/rbt_d_6.png");
                    break;
                case 6:
                    imgtrans->setImage(":/robot/images/rbt_d_7.png");
                    break;
                case 7:
                    imgtrans->setImage(":/robot/images/rbt_d_8.png");
                    break;
                default:
                    imgtrans->setImage(":/robot/images/rbt_d_3.png");
                    break;
                }
                vy += velocity;
                trans->moveBy(0, deltaTime * velocity);
            }
            else if(moving == 3)
            {
                if(x < targetx){ trans->setPos(targetx, targety); moving = 0; }
                x /= pace; x %= 8;
                switch(x){
                case 0:
                    imgtrans->setImage(":/robot/images/rbt_l_1.png");
                    break;
                case 1:
                    imgtrans->setImage(":/robot/images/rbt_l_2.png");
                    break;
                case 2:
                    imgtrans->setImage(":/robot/images/rbt_l_3.png");
                    break;
                case 3:
                    imgtrans->setImage(":/robot/images/rbt_l_4.png");
                    break;
                case 4:
                    imgtrans->setImage(":/robot/images/rbt_l_5.png");
                    break;
                case 5:
                    imgtrans->setImage(":/robot/images/rbt_l_6.png");
                    break;
                case 6:
                    imgtrans->setImage(":/robot/images/rbt_l_7.png");
                    break;
                case 7:
                    imgtrans->setImage(":/robot/images/rbt_l_8.png");
                    break;
                default:
                    imgtrans->setImage(":/robot/images/rbt_l_3.png");
                    break;
                }
                vx -= velocity;
                trans->moveBy(- deltaTime * velocity, 0);
            }
            else if(moving == 4)
            {
                if(x > targetx){ trans->setPos(targetx, targety); moving = 0; }
                x /= pace; x %= 8;
                switch(x){
                case 0:
                    imgtrans->setImage(":/robot/images/rbt_r_1.png");
                    break;
                case 1:
                    imgtrans->setImage(":/robot/images/rbt_r_2.png");
                    break;
                case 2:
                    imgtrans->setImage(":/robot/images/rbt_r_3.png");
                    break;
                case 3:
                    imgtrans->setImage(":/robot/images/rbt_r_4.png");
                    break;
                case 4:
                    imgtrans->setImage(":/robot/images/rbt_r_5.png");
                    break;
                case 5:
                    imgtrans->setImage(":/robot/images/rbt_r_6.png");
                    break;
                case 6:
                    imgtrans->setImage(":/robot/images/rbt_r_7.png");
                    break;
                case 7:
                    imgtrans->setImage(":/robot/images/rbt_r_8.png");
                    break;
                default:
                    imgtrans->setImage(":/robot/images/rbt_r_3.png");
                    break;
                }
                vx += velocity;
                trans->moveBy(deltaTime * velocity, 0);
            }
            //physics->setVelocity(vx, vy);
        }
        return;
    }

    x /= pace; x %= 8;
    y /= pace; y %= 8;

    if(playerID == 0)// 这里复制了太多，比较粪；但是暂时没找到存很多char*的方法
    {
        if(getKeyDown(Qt::Key_Space)){
            if(map->bombCnt[0] < bombSum)
            {
                map->putBomb(i, j, bombGrade, 3, 0);
                say("Quack!炸弹已放置");
            }
            else{ say("放置太多了，现在还不能放炸弹哦"); }
        }
        if(getKey(Qt::Key_A) && l_en) {
            vx -= velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(3); }
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/l_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/l_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/l_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/l_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/l_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/l_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/l_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_D) && r_en) {
            vx += velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(4); }
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/r_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/r_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/r_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/r_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/r_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/r_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/r_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_W) && u_en) {
            vy -= velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(1); }
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/u_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/u_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/u_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/u_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/u_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/u_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/u_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_S) && d_en) {
            vy += velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(2); }
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/d_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/d_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/d_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/d_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/d_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/d_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/d_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            }
        }
    }
    else if(playerID == 1)
    {
        if(getKeyDown(Qt::Key_Return))
        {
            if(map->bombCnt[1] < bombSum){
                map->putBomb(i, j, bombGrade, 3, 1);
                say("Quack!炸弹已放置");
            }
            else{ say("放置太多了，现在还不能放炸弹哦"); }
        }
        if(getKey(Qt::Key_J) && l_en) {
            vx -= velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(3); }
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/l_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/l_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/l_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/l_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/l_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/l_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/l_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_L) && r_en) {
            vx += velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(4); }
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/r_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/r_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/r_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/r_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/r_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/r_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/r_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_I) && u_en) {
            vy -= velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(1); }
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/u_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/u_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/u_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/u_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/u_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/u_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/u_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_K) && d_en) {
            vy += velocity;
            if(bombMove && map->bmb[i][j] != nullptr) { auto b = map->bmb[i][j]->getComponent<Bomb>(); b->setMove(2); }
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/d_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/d_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/d_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/d_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/d_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/d_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/d_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            }
        }
    }
    physics->setVelocity(vx, vy);
}

bool UserController::predict(int i, int j)
{
    // false - dangerous; true - safe;
    if (map->tile[i][j] > 0) return false;
    int newi, newj;
    for (int k = 0; k < 5; k ++)
        if (1 <= j + dx[k] && j + dx[k] <= 20 && 1 <= i + dy[k] && i + dy[k] <= 15)
            if (map->bmbdata[i + dy[k]][j + dx[k]] > 0) return false;
    for (int k = 1; k < 5; k ++)
        for (int d = 2; d <= 4; d ++)// 探测范围可以适量减少一些来减轻程序负担
        {
            newj = j + dx[k] * d; newi = i + dy[k] * d;
            if (1 <= newj && newj <= 20 && 1 <= newi && newi <= 15)
            {
                if(map->bmbdata[newi][newj] >= 4) return false;
            }
        }
    //qDebug("robot: (%d,  %d) is free to go", i, j);
    return true;
}

float UserController::dto(int i, int j, int id)
{
    if(id == 1) return sqrt((i - pl1->i)*(i - pl1->i) + (j - pl1->j)*(j - pl1->j));
    if(id == 2) return sqrt((i - pl2->i)*(i - pl2->i) + (j - pl2->j)*(j - pl2->j));
    return 0;
}

void UserController::setMove(bool emergency)
{
    if (calmTime > 0) { moving = 0; return; }
    //qDebug("robot setMove");
    float minDistance = 1145141919810.0f;// 在保证安全的前提下向玩家靠近
    float distance;
    int k = 0;
    if (!predict(i, j)) emergency = true;
    else if (escaping) { calmTime = 3.2; escaping  = false; }
    if (emergency)// 紧急情况：慌不择路
    {
        if (predict(i, j)) { calmTime = 2.5; return; }// 假如已经安全，等待一会儿，否则它会着急走进炸弹光束里
        //qDebug("emergency!");//
        k = rand() % 4 + 1;
        int initk = k;
        while(map->tile[i + dy[k]][j + dx[k]] > 0  || map->bmb[i + dy[k]][j + dx[k]] != nullptr)// 尽量不走炸弹
        {
            k ++; k %= 5;
            if (k == initk) break;
        }
        k = rand() % 4 + 1;
        initk = k;
        while(map->tile[i + dy[k]][j + dx[k]] > 0)// 没办法了，只能顶着炸弹冲
        {
            k ++; k %= 5;
            if (k == initk) { k = 0; break; }
        }
        int newi = i + dy[k], newj = j + dx[k];
        targetx = 64 * (newj) + 92;
        targety = 48 * (newi) + 64 - 12;
        moving = k;
        escaping = true;
        //return setMove(true);
    }
    if (predict(i - 1, j))
    {
        if (pl1->isAlive && pl2->isAlive) distance = std::min(dto(i - 1, j, 1), dto(i - 1, j, 2));
        else if (!pl1->isAlive) distance = dto(i - 1, j, 2);
        else distance = dto(i - 1, j, 1);
        if (distance < minDistance) { minDistance = distance; k = 1; }
    }
    if (predict(i + 1, j))
    {
        if (pl1->isAlive && pl2->isAlive) distance = std::min(dto(i + 1, j, 1), dto(i + 1, j, 2));
        else if (!pl1->isAlive) distance = dto(i + 1, j, 2);
        else distance = dto(i + 1, j, 1);
        if (distance < minDistance) { minDistance = distance; k = 2; }
    }
    if (predict(i, j - 1))
    {
        if (pl1->isAlive && pl2->isAlive) distance = std::min(dto(i, j - 1, 1), dto(i, j - 1, 2));
        else if (!pl1->isAlive) distance = dto(i, j - 1, 2);
        else distance = dto(i, j - 1, 1);
        if (distance < minDistance) { minDistance = distance; k = 3; }
    }
    if (predict(i, j + 1))
    {
        if (pl1->isAlive && pl2->isAlive) distance = std::min(dto(i, j + 1, 1), dto(i, j + 1, 2));
        else if (!pl1->isAlive) distance = dto(i, j + 1, 2);
        else distance = dto(i, j + 1, 1);
        if (distance < minDistance) { minDistance = distance; k = 4; }
    }
    int newi = i + dy[k], newj = j + dx[k];
    targetx = 64 * (newj) + 92;
    targety = 48 * (newi) + 64 - 12;
    moving = k;
    qDebug("robot %d setMove index to %d", playerID, k);
}

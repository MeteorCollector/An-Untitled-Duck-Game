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
        label->setPlainText("Player 1");
    }
    else if(playerID == 1)
    {
        label->setDefaultTextColor(QColor(0, 0, 255));
        label->setPlainText("Player 2");
    }
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
    effect->setSource(QUrl("qrc:/pr/audios/classic_hurt.wav"));
    effect->play();
    say("被炸到了！好疼啊...");
    if(health <= 0){ return die(); }
}

void UserController::die()
{
    isAlive = false;
    imgtrans->setImage(":/player/images/tomb.png");
    auto p1 = map->player1->getComponent<UserController>();
    auto p2 = map->player2->getComponent<UserController>();
    map->mplr->stop();
    if(!p1->isAlive && !p2->isAlive)
        return map->deathUI();
}

void UserController::onUpdate(float deltaTime) {

    if(!isAlive){ return; }

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

    //label->setPlainText("x = " + QString::number(j) + "  y = " + QString::number(i));

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

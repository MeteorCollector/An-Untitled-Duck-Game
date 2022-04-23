#ifndef SUMMONPLAYER_H
#define SUMMONPLAYER_H

#include <component.h>

class SummonPlayer : public Component {
 public:
  SummonPlayer();
  int spawn_x, spawn_y, id;
  bool is

  void onAttach() override;

  void onClick(QGraphicsSceneMouseEvent *ev) override;
};

#endif // SUMMONPLAYER_H

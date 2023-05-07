#ifndef EXPLOSIONOBJECT_H
#define EXPLOSIONOBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUM_FRAME_EXP 5

class ExplosionObject : public BaseObject{
public:
    ExplosionObject();
    ~ExplosionObject();
public:
    void set_clip();
    void set_frame(const int& fr){frame_=fr;}
    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    int get_frame_width()const{return frame_width_;}
    int get_frame_heigh()const{return frame_height_;}

private:
    int frame_width_;
    int frame_height_;

    unsigned int frame_;
    SDL_Rect frame_clip_[5];
};

#endif // EXPLOSIONOBJECT_H

#ifndef ITERATOR_H
#define ITERATOR_H

class Iterator{
    private:
    public:
        virtual void next() =0;
        virtual void first()=0;
        virtual bool isDone()=0;
        virtual void currentItem()=0;

};

#endif

class Vector2d {
    private:
        double x;
        double y;
    public:
        Vector2d ();
        Vector2d (double a, double b);

        double getX();
        void setX(double x);
        double getY();
        void setY(double y);
        void setVector(double x, double y);

        Vector2d& operator += (Vector2d& right);
        Vector2d& operator /= (double right);
        bool operator == (Vector2d& right);
        bool operator != (Vector2d& right);
};


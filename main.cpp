#include <iostream>

namespace tretyakov
{
  enum Color {RED, GREEN, BLUE}; // ENUM СТАВИТ ИНТЫ по умолчанию(0, 1, 2)
  
  class Planar;
  class Colored;
  class Point;
  class Vector;
  class ColoredPoint;
  class Brush;

  class Planar
  {
  public:
    virtual ~Planar() = default;

    virtual std::ostream& info(std::ostream& out) = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int abs_sqr() const = 0;
  };

  class Colored
  {
  public:
    virtual ~Colored() = default;
    virtual Color getColor() const = 0;
    virtual void setColor(Color color) = 0;
  };

  class Point: public Planar
  {
  public:
    Point(int x, int y):
      x_(x),
      y_(y)
    {}
    virtual ~Point() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Point";
    }

    virtual int x() const override // override - Берем функцию базового класса и ДАЕМ реализацию(перегр.)
    {
      return x_;
    } 
    virtual int y() const override // Не делает ничего сам по себе, прост подсказка, откуда метод
    {
      return y_;
    } 
    virtual int abs_sqr() const override
    {
      return x_ * x_ + y_ * y_;
    }
  private:
    int x_, y_;
  };

  class Vector: public Planar
  {
  public:
    Vector(Point a, Point b):
      a_(a),
      b_(b)
    {}
    virtual ~Vector() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Vector";
    }

    virtual int x() const override
    {
      return b_.x() - a_.x();
    }
    virtual int y() const override
    {
      return b_.y() - a_.y();
    }
    virtual int abs_sqr() const override
    {
      return (x() * x() + y() * y());
    }
  private:
    Point a_;
    Point b_;
  };

  class ColoredPoint: public Planar, public Colored
  {
  public:
    ColoredPoint(Point p, Color color):
      p_(p),
      cl_(color)
    {}
    virtual ~ColoredPoint() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored point";
    }

    virtual int x() const override
    {
      return p_.x();
    }
    virtual int y() const override
    {
      return p_.y();
    }
    virtual int abs_sqr() const override
    {
      return p_.abs_sqr();
    }
    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Point p_;
    Color cl_;
  };

  class Brush: public Colored
  {
  public:
    Brush(Color color): // explicit перед констр. запрещает неявные преобразования
      cl_(color)
    {}
    virtual ~Brush() = default;

    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Color cl_;
  };

  class Circle: public Planar
  {
  public:
    Circle(Point p, int radius):
      center_(p),
      radius_(radius)
    {}
    virtual ~Circle() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Circle";
    }
    virtual int x() const override
    {
      return center_.x();
    }
    virtual int y() const override
    {
      return center_.y();
    }
    virtual int abs_sqr() const override
    {
      return center_.x() * center_.x() + center_.y() * center_.y();
    }
    double sqr() const
    {
      return 3.14 * radius_ * radius_;
    }
    void move(Point p)
    {
      center_ = p;
    }
    void resize(int r)
    {
      if (r > 0)
      {
        radius_ = r;
      }
    }
  private:
    Point center_;
    int radius_;
  };

  class ColoredCircle: public Planar, public Colored
  {
  public:
    ColoredCircle(Point p, int radius, Color color):
      center_(p),
      radius_(radius),
      color_(color)
    {}
    virtual ~ColoredCircle() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored circle";
    }
    virtual int x() const override
    {
      return center_.x();
    }
    virtual int y() const override
    {
      return center_.y();
    }
    virtual int abs_sqr() const override
    {
      return center_.x() * center_.x() + center_.y() * center_.y();
    }
    virtual Color getColor() const override
    {
      return color_;
    }
    virtual void setColor(Color color) override
    {
      color_ = color;
    }
  private:
    Point center_;
    int radius_;
    Color color_;
  };
}

std::ostream& print_abs_sqr(std::ostream& out, tretyakov::Planar * pl)
{
  return pl->info(out) << " " << pl->abs_sqr();
}

void del(tretyakov::Planar * pls[], size_t size)
{
  for (size_t i = 0; i < size - 1; ++i)
  {
    delete pls[i];
  }
}

int main()
{
  const size_t cap = 6;
  tretyakov::Planar * pls[cap];
  size_t size = 0;
  try 
  {
    pls[size++] = new tretyakov::Point(1, 2);
    pls[size++] = new tretyakov::Point(3, 4);
    pls[size++] = new tretyakov::Vector(tretyakov::Point(1, 2), tretyakov::Point(3, 4));
    pls[size++] = new tretyakov::Vector(tretyakov::Point(3, 4), tretyakov::Point(1, 2));
    pls[size++] = new tretyakov::ColoredPoint(tretyakov::Point(1, 2), tretyakov::Color::RED);
    pls[size++] = new tretyakov::ColoredPoint(tretyakov::Point(3, 4), tretyakov::Color::GREEN);
  }
  catch (const std::exception & e)
  {
    del(pls, size);
    std::cerr << e.what() << "\n";
  }

  for (size_t i = 0; i < size; ++i)
  {
    print_abs_sqr(std::cout, pls[i]) << "\n";
  }

  del(pls, size);
  return 0;
}
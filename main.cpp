#include <iostream>

const size_t planarSize = 3;
const int planarTypes[planarSize] = {1, 2, 3};

namespace tretyakov
{
  enum Color {RED, GREEN, BLUE}; // ENUM СТАВИТ ИНТЫ по умолчанию(0, 1, 2 - index)
  
  Color makeColor(char c)
  {
    switch (c)
    { 
    case 'r': return Color::RED;
    case 'g': return Color::GREEN;
    case 'b': return Color::BLUE;
    default: throw std::invalid_argument("Invalid color");
    }
  }

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
  
  Planar * clonePoint(std::istream& in)
  {
    int x = 0, y = 0;
    if (in >> x >> y)
    {
      return new Point(x, y);
    }
    throw std::logic_error("Incorrect Point");
  }

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

  Planar * cloneVector(std::istream& in)
  {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    if (in >> x1 >> y1 >> x2 >> y2)
    {
      return new Vector(Point(x1, y1), Point(x2, y2));
    }
    throw std::logic_error("Incorrect Brush");
  }

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

  ColoredPoint * cloneColoredPoint(std::istream& in)
  {
    int x = 0, y = 0;
    char color = 0;
    if (in >> x >> y >> color)
    {
      return new ColoredPoint(Point(x, y), makeColor(color));
    }
    throw std::logic_error("Incorrect Colored Point");
  }

  Planar * cloneColoredPointPlanar(std::istream& in)
  {
    return cloneColoredPoint(in);
  }

  Colored * cloneColoredPointColored(std::istream& in)
  {
    return cloneColoredPoint(in);
  }

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

  Colored * cloneBrush(std::istream& in)
  {
    char color = 0;
    if (in >> color)
    {
      return new Brush(makeColor(color));
    }
    throw std::logic_error("Incorrect Brush");
  }

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
    ColoredCircle(Point p, int r, Color color):
      cir_(p, r),
      color_(color)
    {}
    virtual ~ColoredCircle() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored circle";
    }
    virtual int x() const override
    {
      return cir_.x();
    }
    virtual int y() const override
    {
      return cir_.y();
    }
    virtual int abs_sqr() const override
    {
      return cir_.abs_sqr();
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
    Circle cir_;
    Color color_;
  };

  std::ostream& print_abs_sqr(std::ostream& out, Planar * pl)
  {
    return pl->info(out) << " " << pl->abs_sqr();
  }

  void del(Planar * pls[], size_t size)
  {
    for (size_t i = 0; i < size - 1; ++i)
    {
      delete pls[i];
    }
  }

  Planar * fabricPlanar(std::istream& in)
  {
    int num = 0;
    in >> num;
    switch (num)
    {
    case 1: return clonePoint(in);
    case 2: return cloneVector(in); 
    case 3: return cloneColoredPointPlanar(in);
    default: throw std::logic_error("Incorrect planar");
    }
  }

  Colored * fabricColored(std::istream& in)
  {
    int num = 0;
    in >> num;
    switch (num)
    {
    case 3: return cloneColoredPointColored(in);
    case 4: return cloneBrush(in);
    default: throw std::logic_error("Incorrect planar");
    }
  }
}

namespace fabric
{
  class AbstractFabricPlanar
  {
  public:
    virtual ~AbstractFabricPlanar() = default;

    virtual tretyakov::Planar * create(std::istream& in) = 0;
  };

  class FabricPoint final: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricPoint() = default;

    virtual tretyakov::Planar * create(std::istream& in) override
    {
      int x = 0, y = 0;
      if (in >> x >> y)
      {
        return new tretyakov::Point(x, y);
      }
      throw std::logic_error("Incorrect Point");
    }
  };

  class FabricVector final: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricVector() = default;

    virtual tretyakov::Planar * create(std::istream& in) override
    {
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      if (in >> x1 >> y1 >> x2 >> y2)
      {
        return new tretyakov::Vector(tretyakov::Point(x1, y1), tretyakov::Point(x2, y2));
      }
      throw std::logic_error("Incorrect Brush");
    }
  };

  class FabricColoredPoint final: public AbstractFabricPlanar
  {
  public:
    virtual ~FabricColoredPoint() = default;

    virtual tretyakov::Planar * create(std::istream& in) override
    {
      int x = 0, y = 0;
      char color = 0;
      if (in >> x >> y >> color)
      {
        return new tretyakov::ColoredPoint(tretyakov::Point(x, y), tretyakov::makeColor(color));
      }
      throw std::logic_error("Incorrect Colored Point");
    }
  };

  void delFabricsPlanar(AbstractFabricPlanar ** fabricsPlanar, size_t size)
  {
    for (size_t j = 0; j < size; j++)
      {
        delete fabricsPlanar[j];
      }
    delete[] fabricsPlanar;
  }

  AbstractFabricPlanar ** makeFabricsPlanar()
  {
    AbstractFabricPlanar ** fabricsPlanar = new AbstractFabricPlanar * [planarSize];
    size_t i = 0;
    try
    {
      fabricsPlanar[i] = new FabricPoint();
      i++;
      fabricsPlanar[i] = new FabricVector();
      i++;
      fabricsPlanar[i] = new FabricColoredPoint();
      i++;
    }
    catch(const std::exception & e)
    {
      delFabricsPlanar(fabricsPlanar, i);
      std::cerr << e.what() << '\n';
    }
    return fabricsPlanar;
  }
}

int main()
{
  size_t cap = 0;
  fabric::AbstractFabricPlanar ** fabrics = fabric::makeFabricsPlanar();
  std::cin >> cap;
  if (!std::cin)
  {
    std::cerr << "Incorrect size\n";
    return 1;
  }
  tretyakov::Planar ** pls = nullptr;
  try
  {
    pls = new tretyakov::Planar * [cap];
  }
  catch(const std::exception & e)
  {
    fabric::delFabricsPlanar(fabrics, planarSize);
    std::cerr << e.what() << '\n';
    return 1;
  }
  
  size_t size = 0;
  while (std::cin)
  {
    int num = 0;
    try
    {
      for (size_t j = 0; j < planarSize; j++)
      {
        if (num == planarTypes[j])
        {
          pls[size] = fabrics[j]->create(std::cin);
          size++;
        }
      }
    }
    catch(const std::exception & e)
    {
      fabric::delFabricsPlanar(fabrics, planarSize);
      tretyakov::del(pls, size);
      delete[] pls;
      std::cerr << e.what() << "\n";
    }
  }

  for (size_t i = 0; i < size; ++i)
  {
    tretyakov::print_abs_sqr(std::cout, pls[i]) << "\n";
  }

  fabric::delFabricsPlanar(fabrics, planarSize);
  tretyakov::del(pls, size);
  delete [] pls;
  return 0;
}

// 1 - Point [x, y]
// 2 - Vector [x1, y1, x2, y2]
// 3 - ColoredPoint [x, y, color]
// 4 - Brush [color]
// 5 - Circle

// 0. count(число фигур)
// 1. [num] [args] Т.е. для точки args x, y
// ...
// count. (От 1 до count вводит фигуры)
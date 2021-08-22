#include <iostream>
#include <limits>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::min()} {
	}

	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}

	double eval() const override {
		return m_min;
	}

	const char * name() const override {
		return "min";
	}

private:
	double m_min;
};

class Max : public IStatistics {
public:
        Max() : m_max{std::numeric_limits<double>::min()} {
        }

        void update(double next) override {
                if (next > m_max) {
                        m_max = next;
                }
        }

        double eval() const override {
                return m_max;
        }

        const char * name() const override {
                return "max";
        }

private:
        double m_max;
};

class Mean : public IStatistics {
public:
  Mean() : m_mean{0}, m_count{1}, m_sum{0}
  {

  }

  void update(double next) override {

      m_sum = (m_sum + next);
      m_mean  = m_sum / m_count;
      //      std::cout << "m_mean " << m_mean << " "
      //                             << m_sum << " "
      //                             << m_count << std::endl;
      ++m_count;
  }

  double eval() const override {
    return m_mean;
  }

  const char * name() const override {
    return "mean";
  }

private:
  double m_sum;
  double m_mean;
  int m_count;
};

class Std : public IStatistics {
public:
  Std() : m_std{0}, m_count{1}, m_sum{0}
  {
  }

  void update(double next) override {

    m_sum = (m_sum + next);
    double m_mean = m_sum / m_count;
    m_dispSum = m_dispSum + ((next - m_mean)*(next - m_mean));

    //! Вопрос: должны ли мы делить на count или count-1?
    //! и как лучше обработать такой случай с делением на 0?
    double disp;
    if(m_count ==1){
       disp = m_dispSum;
    } else {
    disp = m_dispSum / (m_count-1) ;
    }

    m_std = std::sqrtl(disp);
    ++m_count;
  }

  double eval() const override {
    return m_std;
  }

  const char * name() const override {
    return "std";
  }

private:
  double m_dispSum;
  double m_sum;
  double m_std;
  int m_count;
};

int main() {

        const size_t statistics_count = 4;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
        statistics[1] = new Max{};
        statistics[2] = new Mean{};
        statistics[3] = new Std{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}

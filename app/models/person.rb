class Person < ActiveRecord::Base
  has_many :course_terms_people
  has_many :course_terms, through: :course_terms_people do
    def taking
      where('course_terms_people.role = ?', 'student')
    end

    def administrating
      where('course_terms_people.role = ?', 'admin')
    end

    def leading
      where('course_terms_people.role = ?', 'leader')
    end
  end

  has_many :sections_people
  has_many :sections, through: :sections_people do
    def sections_taking
      where('sections_people.student = ?', true)
    end
    def sections_leading
      where('sections_people.student = ?', false)
    end
  end

  has_many :submissions
end

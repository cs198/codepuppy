class Person < ActiveRecord::Base
  has_many :courses_people
  has_many :courses, through: :courses_people do
    def taking
      where('courses_people.role = ?', 'student')
    end

    def administrating
      where('courses_people.role = ?', 'admin')
    end

    def leading
      where('courses_people.role = ?', 'leader')
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

  validates_presence_of :user_system_id
  validates_presence_of :given_name
  validates_presence_of :family_name
end

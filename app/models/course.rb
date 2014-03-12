class Course < ActiveRecord::Base
  has_many :courses_people, class_name: 'CoursesPeople'
  has_many :people, through: :courses_people do
    def students
      where('courses_people.role = ?', 'student')
    end

    def admins
      where('courses_people.role = ?', 'admin')
    end

    def leaders
      where('courses_people.role = ?', 'leader')
    end
  end

  has_many :assignments
  has_many :sections

  validates_presence_of :course_dept
  validates_presence_of :course_code
  validates_presence_of :course_name
  validates_presence_of :term_name
  validates_presence_of :period
  validates_presence_of :active
end

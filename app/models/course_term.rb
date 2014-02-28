class CourseTerm < ActiveRecord::Base
  has_many :course_terms_people
  has_many :people, through: :course_terms_people do
    def students
      where('course_terms_people.role = ?', 'student')
    end

    def admins
      where('course_terms_people.role = ?', 'admin')
    end

    def leaders
      where('course_terms_people.role = ?', 'leader')
    end
  end

  has_many :assignments
  has_many :sections
  belongs_to :course

  validates_presence_of :course_id
  validates_presence_of :course_name
  validates_presence_of :term_name
  validates_presence_of :period
  validates_presence_of :active
end

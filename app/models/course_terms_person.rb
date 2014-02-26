class CourseTermsPerson < ActiveRecord::Base
  belongs_to :course_term
  belongs_to :person
  validates :role, inclusion: { in: ['student', 'leader', 'admin'] }
end

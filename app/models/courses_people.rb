class CoursesPeople < ActiveRecord::Base
  belongs_to :course
  belongs_to :person
  validates :role, inclusion: { in: %w(student leader admin) }
end

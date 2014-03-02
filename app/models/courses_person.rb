class CoursesPerson < ActiveRecord::Base
  belongs_to :course
  belongs_to :person
  validates :role, inclusion: { in: %w(student leader admin) }
end

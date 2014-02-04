class CourseTermsPeople < ActiveRecord::Base
  validates :role, inclusion: { in: ['student', 'leader', 'admin'] }
end

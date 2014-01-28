class CourseTermsPeople < ActiveRecord::Base
  scope :student, -> { where(role: 'student') }
  scope :admin, -> { where(role: 'admin') }
  scope :leader, -> { where(role: 'leader') }
end

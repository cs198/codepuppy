class SectionsPeople < ActiveRecord::Base
  scope :student, -> { where(student: true) }
  scope :leader, -> { where(student: false) }
end

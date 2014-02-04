class SectionsPeople < ActiveRecord::Base
  validates :role, inclusion: { in: ['student', 'leader'] }
end

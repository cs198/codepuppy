class SectionsPeople < ActiveRecord::Base
  belongs_to :section
  belongs_to :person
  validates :role, inclusion: { in: ['student', 'leader'] }
end

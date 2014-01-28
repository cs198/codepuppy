class CreateAssignment < ActiveRecord::Migration
  def change
    create_table :assignments do |t|
      t.belongs_to :course_term, index: true
      t.integer :number
      t.timestamp :date_assigned
      t.timestamp :date_due
    end
  end
end

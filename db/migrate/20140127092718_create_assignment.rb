class CreateAssignment < ActiveRecord::Migration
  def change
    create_table :assignments do |t|
      t.integer :number
      t.timestamp :date_assigned
      t.timestamp :date_due
    end

    add_reference :assignments, :course_term, index: true
  end
end

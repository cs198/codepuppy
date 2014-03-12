class CreateSubmission < ActiveRecord::Migration
  def change
    create_table :submissions do |t|
      t.belongs_to :person, index: true
      t.belongs_to :assignment, index: true
      t.string :location
      t.timestamp :date_submitted
      t.boolean :feedback_released
    end

    add_index :submissions, [:person_id, :assignment_id]
  end
end

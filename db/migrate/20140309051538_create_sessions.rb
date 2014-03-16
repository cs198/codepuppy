class CreateSessions < ActiveRecord::Migration
  def change
    create_table :sessions do |t|
      t.string :token, index: true
      t.belongs_to :person, index: true
      t.boolean :verified
      t.timestamp :last_seen_at
      t.timestamps
    end
  end
end

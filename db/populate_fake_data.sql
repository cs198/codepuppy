-- Clear all rows from all tables.
-- WARNING: THIS LITERALLY DESTROYS ALL THE DATA IN ALL TABLES.
-- NEVER RUN ONCE THE SITE IS LIVE.

DELETE FROM comments;
DELETE FROM submissions;
DELETE FROM assignments;

DELETE FROM sections_people;
DELETE FROM sections;

DELETE FROM courses_people;
DELETE FROM courses;
DELETE FROM people;

ALTER SEQUENCE comments_id_seq RESTART;
ALTER SEQUENCE submissions_id_seq RESTART;
ALTER SEQUENCE assignments_id_seq RESTART;

ALTER SEQUENCE sections_people_id_seq RESTART;
ALTER SEQUENCE sections_id_seq RESTART;

ALTER SEQUENCE courses_people_id_seq RESTART;
ALTER SEQUENCE courses_id_seq RESTART;
ALTER SEQUENCE people_id_seq RESTART;

-- Create fake data.

INSERT INTO people (user_system_id, given_name, family_name) VALUES
('amainero', 'Anthony', 'Mainero'),
('maesenc', 'Maesen', 'Churchill'),
('odiab', 'Omar Sebastian', 'Diab'),
('rawatson', 'Reid', 'Watson');

INSERT INTO courses (course_dept, course_code, course_name, term_name, period, active) VALUES
('CS', '106A', 'Programming Methodology', 'Fall', '13-14', False),
('CS', '106B', 'Programming Abstractions', 'Fall', '13-14', False),
('CS', '106A', 'Programming Methodology', 'Winter', '13-14', True),
('CS', '106B', 'Programming Abstractions', 'Winter', '13-14', True);

-- For CS106A Winter 2014, Reid is admin, Maesen leader, Omar and Anthony students.

INSERT INTO courses_people (course_id, person_id, role) VALUES
(3, 4, 'admin'),
(3, 2, 'leader'),
(3, 1, 'student'),
(3, 3, 'student');

-- Create a new section for CS106A Winter 2014.

INSERT INTO sections (course_id) VALUES
(3);

-- Maesen teaches a section consisting of Omar and Anthony.

INSERT INTO sections_people (section_id, person_id, role) VALUES
(1, 2, 'leader'),
(1, 1, 'student'),
(1, 3, 'student');

-- Create a new assignment for CS106A Winter 2014.

INSERT INTO assignments (course_id, number, date_assigned, date_due) VALUES
(3, 1, '2014-02-06', '2014-03-14 11:59:59');

-- Create new submissions for this assignment.

INSERT INTO submissions (student_id, assignment_id, location,
                        date_submitted, feedback_released) VALUES
(1, 1, 'anthony_fake_submission', '2014-02-05 11:00:00', False),
(3, 1, 'omar_fake_submission', '2014-02-05 11:30:00', True);




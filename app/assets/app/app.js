//= require_self
//= require_tree .

codepuppy = angular.module('codepuppy', ['ngRoute', 'ui.bootstrap', 
  'angularFileUpload']);

codepuppy.config(function($routeProvider) {
  // TODO: use session information to determine role.  Should be one of 'student', 'leader'
  var role = 'leader';

  // Student-specific routes:
  if(role === 'student') {
    $routeProvider.when('/:courseID/:assignmentID/submit', {
      templateUrl: '/assets/pages/assignment/' + role + '/assignment.html',
      controller: 'AssignmentCtrl'
    });
  }

  $routeProvider
  .when('/', {
    templateUrl: '/assets/pages/home/' + role + '/home.html',
    controller: 'HomeCtrl'
  })
  .when('/:courseID', {
    templateUrl: '/assets/pages/course/' + role + '/course.html',
    controller: 'CourseCtrl'
  })
  .when('/:courseID/:assignmentID', {
    templateUrl: '/assets/pages/assignment/' + role + '/assignment.html',
    controller: 'AssignmentCtrl'
  })
  .when('/:courseID/:assignmentID/:submissionID', {
    templateUrl: '/assets/pages/submission/' + role + '/submission.html',
    controller: 'SubmissionCtrl'
  });

  

});

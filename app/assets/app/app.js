//= require_self
//= require_tree .

codepuppy = angular.module('codepuppy', ['ngRoute', 'ui.bootstrap',
  'angularFileUpload']);

codepuppy.config(function($routeProvider) {
  // TODO: use session information to determine role.  Should be one of 'student', 'leader'
  var role = 'student';
  var capRole = 'Student';

  // Student-specific routes:
  if(role === 'student') {
    $routeProvider.when('/:courseID/:assignmentID/submit', {
      templateUrl: '/assets/pages/fileUpload/' + role + '/upload.html',
      controller: 'FileUploadCtrl'
    });
  }

  $routeProvider
  .when('/', {
    templateUrl: '/assets/pages/home/' + role + '/' + role + 'Home.html',
    controller: capRole + 'HomeCtrl'
  })
  .when('/courses/:courseID', {
    templateUrl: '/assets/pages/course/' + role +'/' + role + 'Course.html',
    controller: capRole + 'CourseCtrl'
  })
  .when('/courses/:courseID/assignments/:assignmentID', {
    templateUrl: '/assets/pages/assignment/' + role + '/' + role + 'Assignment.html',
    controller: capRole + 'AssignmentCtrl'
  })
  .when('/courses/:courseID/assignments/:assignmentID/' +
        'submissions/:submissionID',
  {
    templateUrl: '/assets/pages/submission/' + role + '/' + role + 'Submission.html',
    controller: capRole + 'SubmissionCtrl'
  })
  .when('/sessions/verify', {
    templateUrl: '/assets/pages/sessions/verify/sessions-verify.html',
    controller: 'SessionsVerifyCtrl'
  })
  .otherwise({
    templateUrl: '/assets/errorPages/404/404.html',
    controller: '404Ctrl'
  });



});

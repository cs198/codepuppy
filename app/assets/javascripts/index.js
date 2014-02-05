codepuppy = angular.module('codepuppy', ['ngRoute']);

codepuppy.config(function($routeProvider) {
  //if ($user == 'student') {
  if (true) {
    $routeProvider
    .when('/', {
      templateUrl : '../pages/home.html',
      controller  : 'mainController'
    })
    .when('/:classID', {
      templateUrl : '../pages/class.html',
      controller  : 'classController'
    })
    .when('/:classID/:assignmentID', {
      templateUrl : '../pages/assignment.html',
      controller  : 'assignmentController'
    })

//} else if ($user == 'section leader') {
//     more stuff
//} else if ($user == 'lecturer') {
//     more stuff

  }
});

codepuppy.controller('classController', function($scope, $routeParams) {
    $scope.message = 'Look! I am a class page for ' + $routeParams.classID;
    $scope.$parent.classes = [$routeParams.classID];
    var FAKEassignmentsForGivenClass = ['One', 'Two']
    $scope.$parent.assignments = FAKEassignmentsForGivenClass;
});

codepuppy.controller('assignmentController', function($scope, $routeParams) {
    $scope.$parent.classes = [$routeParams.classID];
    $scope.$parent.assignments = [$routeParams.assignmentID];
    $scope.message = "I'm an assignment page for the class " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
});


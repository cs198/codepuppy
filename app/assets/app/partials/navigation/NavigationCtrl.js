angular.module('codepuppy').controller('NavigationCtrl', function($scope) {
  //TODO: make these api calls
  var courses = ['cs106a', 'cs106b', 'cs142'];
  var assignments = ['Assignment1', 'Assignment2'];
  var submissions = ['rawatson_1', 'rawatson_2', 'maesenc_1'];

  $scope.navigationSelectors = [
    { title: 'Courses', elements: courses },
    { title: 'Assignments', elements: assignments},
    { title: 'Submissions', elements: submissions }
  ];
});

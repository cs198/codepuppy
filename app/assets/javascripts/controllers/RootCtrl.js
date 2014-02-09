angular.module('codepuppy').controller('RootCtrl', function($scope) {
  var FAKEcourses = ['cs106a', 'cs106b', 'cs106x', 'cs142'];
  $scope.courses = FAKEcourses;
});

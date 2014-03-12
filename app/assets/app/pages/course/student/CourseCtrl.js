angular.module('codepuppy').controller('CourseCtrl', function($scope, $routeParams) {
    $scope.message = 'Look! I am a course page for ' + $routeParams.courseID;
});

angular.module('codepuppy').controller('LeaderCourseCtrl', function($scope, $routeParams) {
    $scope.message = 'Look! I am a course page for ' + $routeParams.courseID;
});

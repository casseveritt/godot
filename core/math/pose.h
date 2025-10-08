/**************************************************************************/
/*  pose.h                                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "core/math/math_funcs.h"
#include "core/math/quaternion.h"
#include "core/math/vector3.h"
#include "core/string/ustring.h"

struct [[nodiscard]] Pose {
	Quaternion rotation;
	Vector3 translation;

	bool is_equal_approx(const Pose &p_pose) const;
	bool is_same(const Pose &p_pose) const;
	bool is_finite() const;
	bool is_valid() const;
	Pose inverse() const;

	constexpr void operator*=(const Pose &p_p);
	constexpr Pose operator*(const Pose &p_p) const;

	_FORCE_INLINE_ Vector3 xform(const Vector3 &p_v) const {
		return translation + rotation.xform(p_v);
	}

	_FORCE_INLINE_ Vector3 xform_inv(const Vector3 &p_v) const {
		return inverse().xform(p_v);
	}

	constexpr bool operator==(const Pose &p_pose) const;
	constexpr bool operator!=(const Pose &p_pose) const;

	explicit operator String() const;

	constexpr Pose() :
			rotation(), translation() {}

	constexpr Pose(const Quaternion &p_r, const Vector3 &p_t) :
			rotation(p_r), translation(p_t) {}


	constexpr Quaternion(const Pose &p_p) :
			rotation(p_p.rotation), translation(p_p.translation) {}

	constexpr void operator=(const Pose &p_p) {
		rotation = p_p.rotation;
		translation = p_p.translation;
	}
};

_FORCE_INLINE_ bool Pose::is_equal_approx(const pose &p_pose) const {
	return rotation.is_equal_approx(p_pose.rotation) && translation.is_equal_approx(p_pose.translation);
}

_FORCE_INLINE_ bool Pose::is_same(const pose &p_pose) const {
	return rotation == p_pose.rotation && translation == p_pose.translation;
}

_FORCE_INLINE_ bool Pose::is_finite() const {
	return rotation.is_finite() && translation.is_finite();
}

_FORCE_INLINE_ Pose Pose::inverse() const {
	Quaternion invrot = rotation.inverse();
	return Pose(invrot, invrot.xform(-translation));
}

_FORCE_INLINE_ Pose::operator String() const {
	return "(" + String(rotation) + ", " + String(translation) + ")";
}

constexpr bool Pose::operator==(const Pose &p_pose) const {
	return rotation == p_pose.rotation && translation == p_pose.translation;
}

constexpr bool Pose::operator!=(const Pose &p_pose) const {
	return rotation != p_pose.rotation || translation != p_pose.translation;
}

constexpr void Pose::operator*=(const Pose &p_p) {
	translation = xform(p_p.translation);
	rotation *= p_p.rotation;
}

constexpr Pose Pose::operator*(const Pose &p_p) const {
	Pose p = *this;
	p *= p_p;
	return p;
}

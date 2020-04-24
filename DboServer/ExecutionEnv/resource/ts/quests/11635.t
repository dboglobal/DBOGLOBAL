CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11635;
	title = 1100949;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100951;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100953;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100945;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 2;
			mlt = 20000;
			os = 0;
			prelnk = "3;";
			dg = 1100954;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 5;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100948;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 12;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "10;10;";
			dg = 1100955;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "8;";
			nolnk = 255;
			rm = 0;
			yeslnk = 10;

			CDboTSRcvSvrEvt
			{
				id = 16320;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 33;
			prelnk = "11;11;";

			CDboTSActSendSvrEvt
			{
				id = 16280;
				tblidx = 6201101;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActSendSvrEvt
			{
				id = 16270;
				tblidx = 11604;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 16260;
				tblidx = 11604;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100946;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 16290;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSSkillUse
			{
				idx = -1;
				rp = 127;
				stype = 6;
				tidx = 7451107;
				ttype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;3;1;4;2;";

			CDboTSActRegQInfo
			{
				cont = 1100947;
				gtype = -1;
				area = 1100407;
				goal = 1100950;
				grade = -1;
				rwd = 255;
				sort = 1100406;
				stype = 32;
				taid = 1;
				title = 1100949;
			}
		}
		CDboTSContGAct
		{
			cid = 33;
			elnk = 255;
			nextlnk = 254;
			prelnk = "12;";

			CDboTSActSendSvrEvt
			{
				id = 16190;
				tblidx = 11604;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 16200;
				tblidx = -1;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "33;";
			type = 1;
		}
	}
}


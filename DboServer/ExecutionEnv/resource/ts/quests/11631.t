CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11631;
	title = 1100967;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActSendSvrEvt
			{
				id = 17050;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "5;4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = 1701101;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;1;";

			CDboTSActRegQInfo
			{
				cont = 1100965;
				gtype = -1;
				area = 1100648;
				goal = 1100968;
				grade = -1;
				rwd = 255;
				sort = 1100406;
				stype = 32;
				taid = 1;
				title = 1100967;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSSkillUse
			{
				idx = -1;
				rp = 0;
				stype = 0;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100963;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 1701101;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 11632;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "9;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "4;";
			dg = 1100970;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "7;3;";
			dg = 1100969;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 16930;
				tblidx = 1701101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "14;";

			CDboTSActBroadMsg
			{
				dt = 10;
				et = 2;
				oidx = 7511101;
				oqtidx = 1100742;
				ot = 0;
				uisdt = 0;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 0;
				uidt = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 2;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 1;
			prelnk = "0;";
			dg = 1100964;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 15;
			prelnk = "11;12;13;13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
			CDboTSActSendSvrEvt
			{
				id = 17030;
				tblidx = 11606;
				stradius = 0;
				stype = 2;
				taid = 3;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 16940;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 17;
			prelnk = "15;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100600;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 14;
			cid = 12;
			lilnk = 255;
			ndesc0 = 1100601;
			uspt = 1;
			desc = -1;
			nid0 = 13;
			type = -1;
			ust = 1;
			idx = -1;
			prelnk = "11;";
		}
		CDboTSContGAct
		{
			cid = 17;
			elnk = 255;
			nextlnk = 254;
			prelnk = "16;";

			CDboTSActDirIndicator
			{
				dtype = 2;
				self = 1;
				y = "22.070000";
				stype = 1;
				taid = 2;
				x = "-190.050003";
				z = "-24.549999";
			}
			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100604;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "17;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 13;
			dt = 0;
			lilnk = 255;
			oklnk = 14;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "12;";
			dg = 1100602;
			gt = 0;
			oi = -1;
		}
	}
}


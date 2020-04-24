CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11636;
	title = 1100960;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 11;
			prelnk = "9;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "6;7;8;9;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSClickNPC
			{
				npcidx = 2991101;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSSkillUse
			{
				idx = -1;
				rp = 0;
				stype = 1;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100958;
				gtype = -1;
				area = 1100636;
				goal = 1100961;
				grade = -1;
				rwd = 255;
				sort = 1100406;
				stype = 32;
				taid = 1;
				title = 1100960;
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
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "3;2;";
			dg = 1100962;
			gt = 0;
			oi = 2991101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 7;
			dt = 0;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100597;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 18;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100599;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 8;
			dt = 0;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100598;
			gt = 0;
			oi = 2991101;
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
			prelnk = "18;";
			dg = 1100600;
			gt = 0;
			oi = -1;
		}
		CDboTSContGAct
		{
			cid = 17;
			elnk = 255;
			nextlnk = 254;
			prelnk = "16;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100604;
			}
			CDboTSActDirIndicator
			{
				dtype = 2;
				self = 1;
				y = "-91.980003";
				stype = 1;
				taid = 2;
				x = "391.320007";
				z = "146.020004";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 16620;
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
				oqtidx = 1100603;
				ot = 0;
				uisdt = 0;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 0;
				uidt = 0;
			}
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
				id = 16581;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
	}
}


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 1;
	tid = 850;
	title = 85002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 85007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "1653106;";
			}
			CDboTSCheckClrQst
			{
				and = "843;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "11;";
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSClickNPC
			{
				npcidx = "1653106;";
			}
		}
		CDboTSContEnd
		{
			cid = 249;
			prelnk = "100;101;102;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 11;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 85008;
				gtype = 1;
				area = 85001;
				goal = 85004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 85005;
				stype = 2;
				taid = 1;
				title = 85002;
			}
		}
		CDboTSContReward
		{
			canclnk = 249;
			cid = 101;
			rwdzeny = 0;
			desc = 85014;
			nextlnk = 13;
			rwdexp = 0;
			rwdtbl = 85002;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;
		}
		CDboTSContReward
		{
			canclnk = 249;
			cid = 102;
			rwdzeny = 0;
			desc = 85014;
			nextlnk = 14;
			rwdexp = 0;
			rwdtbl = 85003;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 250;
			prelnk = "102;";

			CDboTSActPortal
			{
				dx = "0.740000";
				px = "422.000000";
				widx = 3;
				dy = "0.000000";
				py = "0.000000";
				taid = 1;
				type = 0;
				dz = "0.520000";
				pz = "-438.000000";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 3;
			cid = 11;
			lilnk = 255;
			ndesc0 = 85020;
			uspt = -1;
			desc = 85009;
			nid2 = 102;
			type = 0;
			ust = 0;
			idx = 1653106;
			ndesc2 = 85022;
			nid1 = 101;
			prelnk = "3;2;";
			ndesc1 = 85021;
			nid0 = 100;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 85008;
			gtype = 1;
			oklnk = 2;
			area = 85001;
			goal = 85004;
			sort = 85005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 85002;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActPortal
			{
				dx = "0.740000";
				px = "465.000000";
				widx = 3;
				dy = "0.000000";
				py = "0.000000";
				taid = 1;
				type = 0;
				dz = "0.520000";
				pz = "522.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 249;
			cid = 100;
			rwdzeny = 0;
			desc = 85014;
			nextlnk = 12;
			rwdexp = 0;
			rwdtbl = 85001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 251;
			prelnk = "101;";

			CDboTSActPortal
			{
				dx = "0.740000";
				px = "497.000000";
				widx = 3;
				dy = "0.000000";
				py = "0.000000";
				taid = 1;
				type = 0;
				dz = "0.520000";
				pz = "138.000000";
			}
		}
		CDboTSContEnd
		{
			cid = 250;
			prelnk = "14;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 251;
			prelnk = "13;";
			type = 1;
		}
	}
}


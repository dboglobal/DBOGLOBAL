CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 1;
	tid = 870;
	title = 87002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;102;101;100;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "1653105;";
			}
		}
		CDboTSContEnd
		{
			cid = 250;
			prelnk = "105;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 251;
			prelnk = "104;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "103;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 250;
			prelnk = "102;";

			CDboTSActPortal
			{
				dx = "0.740000";
				px = "360.000000";
				widx = 3;
				dy = "0.000000";
				py = "0.000000";
				taid = 1;
				type = 0;
				dz = "0.520000";
				pz = "-133.000000";
			}
		}
		CDboTSContGAct
		{
			cid = 103;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 104;
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
		CDboTSContUsrSel
		{
			cancellnk = 4;
			cid = 3;
			lilnk = 255;
			ndesc0 = 87020;
			uspt = -1;
			desc = -1;
			nid2 = 102;
			type = 0;
			ust = 0;
			idx = 1653105;
			ndesc2 = 87022;
			nid1 = 101;
			prelnk = "4;2;";
			ndesc1 = 87021;
			nid0 = 100;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 87009;
				ctype = 1;
				idx = 1653105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 87008;
				gtype = 1;
				area = 87001;
				goal = 87004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 87005;
				stype = 1;
				taid = 1;
				title = 87002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 87007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSCheckClrQst
			{
				and = "843;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1653105;";
			}
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 102;
			rwdzeny = 0;
			desc = 87014;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 87003;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 101;
			rwdzeny = 0;
			desc = 87014;
			nextlnk = 104;
			rwdexp = 0;
			rwdtbl = 87002;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 87008;
			gtype = 1;
			oklnk = 2;
			area = 87001;
			goal = 87004;
			sort = 87005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 87002;
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 87014;
			nextlnk = 103;
			rwdexp = 0;
			rwdtbl = 87001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;
		}
	}
}


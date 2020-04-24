CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1834;
	title = 183402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 183407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSClickNPC
			{
				npcidx = "3142209;";
			}
			CDboTSCheckClrQst
			{
				and = "999;";
				flink = 1;
				flinknextqid = "1838;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 183409;
				ctype = 1;
				idx = 3142209;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6013105;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 183408;
				m1fx = "802.070007";
				m1fy = "0.000000";
				sort = 183405;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-859.880005";
				grade = 132203;
				m0fx = "723.940002";
				m0ttip = 183415;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 183402;
				gtype = 2;
				area = 183401;
				goal = 183404;
				m0fz = "-1197.880005";
				m0widx = 1;
				m1ttip = 183415;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 183408;
			gtype = 2;
			oklnk = 2;
			area = 183401;
			goal = 183404;
			sort = 183405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 183402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 183414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 183401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142209;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}


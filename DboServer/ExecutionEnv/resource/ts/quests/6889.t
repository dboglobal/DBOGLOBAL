CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6889;
	title = 688902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 6888;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "7092110;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "6890;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688914;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 688901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7092110;";
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
				itype = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688909;
				ctype = 1;
				idx = 7092110;
				taid = 3;
			}
			CDboTSActRegQInfo
			{
				cont = 688908;
				m1fx = "7486.649902";
				m2fz = "1846.209961";
				m1fy = "0.000000";
				sort = 688905;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 688901;
				goal = 688904;
				m0fz = "1711.569946";
				m0widx = 1;
				m1ttip = 688915;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1828.510010";
				m2fx = "7228.700195";
				m2ttip = 688915;
				m0fx = "7492.029785";
				m0ttip = 688915;
				title = 688902;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1604;
				cnt0 = 12;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688908;
			gtype = 1;
			oklnk = 2;
			area = 688901;
			goal = 688904;
			sort = 688905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688902;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1604;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}


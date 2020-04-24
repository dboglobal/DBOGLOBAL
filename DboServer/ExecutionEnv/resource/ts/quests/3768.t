CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3768;
	title = 376802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 376809;
				ctype = 1;
				idx = 3333101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 12953102;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 12953200;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 376808;
				m3widx = 15;
				m1fy = "0.000000";
				sort = 376805;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-4157.470215";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				area = 376801;
				goal = 376804;
				m0fz = "-6740.399902";
				m0widx = 15;
				m1ttip = 376816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-6764.379883";
				m3ttip = 376816;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-6486.330078";
				m2fx = "-3745.209961";
				m2ttip = 376815;
				m0fx = "-3804.919922";
				m0ttip = 376815;
				m3pat = 2;
				title = 376802;
				m1fx = "-4252.419922";
				m2fz = "-6581.379883";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3333101;";
			}
			CDboTSCheckClrQst
			{
				and = "3767;";
				flink = 0;
				flinknextqid = "3769;";
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 376808;
			gtype = 3;
			oklnk = 2;
			area = 376801;
			goal = 376804;
			sort = 376805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 376802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 376801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 1515;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}


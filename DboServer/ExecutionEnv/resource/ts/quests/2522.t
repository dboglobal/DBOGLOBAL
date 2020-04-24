CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2522;
	title = 252202;

	CNtlTSGroup
	{
		gid = 0;

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
				eitype = 3;
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "8512154;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2523;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 252201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "8512154;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 252209;
				ctype = 1;
				idx = 8512154;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 3;
				eitype = 3;
				idx0 = 1375;
				cnt0 = 3;
				cnt2 = 3;
				ectype = -1;
				etype = 0;
				idx1 = 1376;
				taid = 3;
				esctype = 0;
				idx2 = 1377;
			}
			CDboTSActRegQInfo
			{
				cont = 252208;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-394.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-389.000000";
				m4ttip = 252215;
				area = 252201;
				goal = 252204;
				m0fz = "-890.000000";
				m0widx = 14;
				m1ttip = 252215;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-831.000000";
				m3ttip = 252215;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-522.000000";
				m2fx = "-555.000000";
				m2ttip = 252215;
				m4widx = 14;
				m0fx = "-781.000000";
				m0ttip = 252215;
				m3pat = 2;
				m4pat = 2;
				title = 252202;
				m1fx = "-724.000000";
				m2fz = "-837.000000";
				sort = 252205;
				m4fz = "-531.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 252208;
			gtype = 2;
			oklnk = 2;
			area = 252201;
			goal = 252204;
			sort = 252205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252202;
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
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 1375;
				iidx2 = 1377;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 1376;
				iprob0 = "1.000000";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}


CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2632;
	title = 263202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 102;
			elnk = 253;
			nextlnk = 254;
			prelnk = "101;";

			CDboTSActWPSFD
			{
				sid = 2632;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 263209;
				ctype = 1;
				idx = 5313103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 263208;
				gtype = 1;
				area = 263201;
				goal = 263204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 263205;
				stype = 1;
				taid = 1;
				title = 263202;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 1263;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1262;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 263214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 263201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1262;
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1263;
			}
			CDboTSClickNPC
			{
				npcidx = "5313102;";
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
			prelnk = "102;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 263208;
			gtype = 1;
			oklnk = 2;
			area = 263201;
			goal = 263204;
			sort = 263205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 263202;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "102;1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 263207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5313103;";
			}
			CDboTSCheckClrQst
			{
				and = "2631;";
				flink = 1;
				flinknextqid = "2633;";
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 263207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
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
				iidx1 = 1262;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1263;
				iprob1 = "1.000000";
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
	}
}


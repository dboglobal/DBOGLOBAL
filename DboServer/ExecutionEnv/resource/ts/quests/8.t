CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 8;
	title = 802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 809;
				ctype = 1;
				idx = 8512101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 808;
				gtype = 1;
				area = 801;
				goal = 804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 805;
				stype = 64;
				taid = 1;
				title = 802;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 808;
			gtype = 1;
			oklnk = 2;
			area = 801;
			goal = 804;
			sort = 805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 802;
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8511101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckClrQst
			{
				and = "7;";
				flink = 1;
				flinknextqid = "9;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "8512101;";
			}
		}
	}
}


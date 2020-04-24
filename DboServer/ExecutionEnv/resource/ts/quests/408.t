CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 408;
	title = 40802;

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
				conv = 40809;
				ctype = 1;
				idx = 7131104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 40808;
				gtype = 1;
				area = 40801;
				goal = 40804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 40805;
				stype = 64;
				taid = 1;
				title = 40802;
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "7131104;";
			}
			CDboTSCheckClrQst
			{
				and = "407;";
				flink = 1;
				flinknextqid = "409;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 40808;
			gtype = 1;
			oklnk = 2;
			area = 40801;
			goal = 40804;
			sort = 40805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 40801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1111105;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 377;
	title = 37702;

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
				conv = 37709;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 37708;
				gtype = 1;
				area = 37701;
				goal = 37704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 37705;
				stype = 2;
				taid = 1;
				title = 37702;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 3771;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 37707;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				and = "375;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 37708;
			gtype = 1;
			oklnk = 2;
			area = 37701;
			goal = 37704;
			sort = 37705;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 37702;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 37714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 37701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511102;";
			}
		}
	}
}


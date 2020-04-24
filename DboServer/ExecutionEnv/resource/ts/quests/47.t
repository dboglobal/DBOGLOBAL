CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 47;
	title = 4702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 471;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 4709;
				ctype = 1;
				idx = 4511504;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 4708;
				gtype = 1;
				area = 4701;
				goal = 4704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 4705;
				stype = 2;
				taid = 1;
				title = 4702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 4707;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 9;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4511504;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "48;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 4708;
			gtype = 1;
			oklnk = 2;
			area = 4701;
			goal = 4704;
			sort = 4705;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 4702;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			desc = 4714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 4701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072102;";
			}
		}
	}
}


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 392;
	title = 39202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 39208;
			gtype = 1;
			oklnk = 2;
			area = 39201;
			goal = 39204;
			sort = 39205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 39202;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 39209;
				ctype = 1;
				idx = 4752101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 39208;
				gtype = 1;
				area = 39201;
				goal = 39204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 39205;
				stype = 1;
				taid = 1;
				title = 39202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 39214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 39201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 39207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
			}
			CDboTSCheckClrQst
			{
				and = "305;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
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


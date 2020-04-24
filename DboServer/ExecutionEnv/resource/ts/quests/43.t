CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 43;
	title = 4302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 2;
			cid = 1;
			idx = 431;
			lilnk = 255;
			oklnk = 2;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 100;
			prelnk = "2;";

			CDboTSActNPCConv
			{
				conv = 4309;
				ctype = 1;
				idx = 4511101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 4308;
				gtype = 1;
				area = 4301;
				goal = 4304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 4305;
				stype = 2;
				taid = 1;
				title = 4302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 4307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 8;
			}
			CDboTSClickNPC
			{
				npcidx = "4511101;";
			}
			CDboTSCheckClrQst
			{
				and = "42;";
				flink = 1;
				flinknextqid = "194;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;2;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 2;
			cont = 4308;
			gtype = 1;
			oklnk = 3;
			area = 4301;
			goal = 4304;
			sort = 4305;
			prelnk = "1;1;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 4302;
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
			prelnk = "3;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 4314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 4301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3174101;";
			}
		}
	}
}


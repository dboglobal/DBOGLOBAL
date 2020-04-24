CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 427;
	title = 42702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 6;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 24;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 100;
			prelnk = "8;";

			CDboTSActWorldPlayScript
			{
				sid = 427;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;7;";

			CDboTSActSendSvrEvt
			{
				id = 21;
				tblidx = 427;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 427;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 42709;
				ctype = 1;
				idx = 4261101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 42708;
				gtype = 1;
				area = 42701;
				goal = 42704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 42705;
				stype = 2;
				taid = 1;
				title = 42702;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 8;
			cid = 7;
			idx = 4272;
			lilnk = 255;
			oklnk = 8;
			prelnk = "6;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 42707;
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
				minlvl = 6;
			}
			CDboTSCheckClrQst
			{
				and = "426;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4261101;";
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
			cont = 42708;
			gtype = 1;
			oklnk = 2;
			area = 42701;
			goal = 42704;
			sort = 42705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 42702;
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
			prelnk = "3;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 42714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 42701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "9;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4261101;";
			}
		}
	}
}

